/*
  MaerklinMotorola.cpp - Library for decoding the signals from the Märklin-Motorola-protocol. 
  Created by Michael Henzler (Laserlicht), Februar 27, 2018.
  Released under BSD 2-Clause "Simplified" License.
*/

#include <MaerklinMotorola.h>

MaerklinMotorola::MaerklinMotorola(int p) {
  pin = p;
  DataAvailable = false;
}
void MaerklinMotorola::PinChange() {
  //bool state = digitalRead(pin);
  unsigned long tm = micros();
  unsigned long tm_delta = tm - last_tm;

  if(sync) { //erst nach syncronisation bits sammeln
    Data.Timings[timings_pos] = int(tm_delta); //ablage des zeitunterschieds zwischen den letzten flanken
    timings_pos++;

    if(timings_pos==35) {
      unsigned long tm_package_delta = tm - sync_tm; //paket-laenge berechen
      int period = Data.Timings[0]+Data.Timings[1]; //bit-laenge berechnen
      bool valid = true;
      
      for(int i=0;i<35;i+=2) { //Bits dekodieren
        Data.Bits[i/2] = (Data.Timings[i]>(period>>1)) ? 1 : 0;

        if(i<33) {
          int period_tmp = Data.Timings[i] + Data.Timings[i+1];
          if(period_tmp > 125 && period_tmp < 175) valid = false; //MFX herausfiltern
        }
      }

      for(int i=0;i<9;i++) { //Trits aus Bits dekodieren
        Data.Trits[i] = (Data.Bits[i*2] == 1 && Data.Bits[i*2+1] == 1) ? 1 : ((Data.Bits[i*2] == 0 && Data.Bits[i*2+1] == 0) ? 0 : 2);
      }

      //Decoder
      if(tm_package_delta > 1300 && tm_package_delta < 4200 && valid) { //Protokollspezifische Telegramlänge: Weichen oder Lokprotokoll
        Data.IsMagnet = ((period < 150) ? true : false);  //Unterscheidung Protokoll (Fest-Zeit)
        
        Data.Address = Data.Trits[3] * 27 + Data.Trits[2] * 9 + Data.Trits[1] * 3 + Data.Trits[0];

        if(!Data.IsMagnet) { //Loktelegramm
          Data.Function = (Data.Trits[4] == 1) ? true : false;

          int s = Data.Bits[10] + Data.Bits[12] * 2 + Data.Bits[14] * 4 + Data.Bits[16] * 8;
          Data.Stop = (s==0) ? true : false;
          Data.ChangeDir = (s==1) ? true : false;
          Data.Speed = (s==0) ?  0 : s-1;

          DataAvailable=true;
        } else { //Magnettelegramm
          if(Data.Trits[4]==0) {
            int s = Data.Bits[10] + Data.Bits[12] * 2 + Data.Bits[14] * 4;
            Data.SubAddress = s;
            Data.MagnetState = (Data.Bits[16]==1) ? true : false;
            
            DataAvailable=true;
          }
        }  
      }
      sync = false;
      timings_pos = 0;
    }
  } else {
    if(tm_delta>500) { //Protokollspezifische Pausen-Zeit
      sync = true;
      sync_tm = tm;
    }
  }

  last_tm = tm;
}
