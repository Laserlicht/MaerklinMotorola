/*
  MaerklinMotorola.cpp - Library for decoding the signals from the Märklin-Motorola-protocol. 
  Created by Michael Henzler (Laserlicht), Februar 27, 2018.
  Released under BSD 2-Clause "Simplified" License.
*/

#include <MaerklinMotorola.h>

MaerklinMotorola::MaerklinMotorola(int p) {
  pin = p;
  DataQueueWritePosition = 0;
  sync = false;
}

MaerklinMotorolaData* MaerklinMotorola::GetData() {
	for(int QueuePos=0; QueuePos<MM_QUEUE_LENGTH;QueuePos++) {
		if(DataGramState_Validated == DataQueue[QueuePos].State) {
			DataQueue[QueuePos].State = DataGramState_Finished;
			return DataQueue + QueuePos;
		}
	}
	return 0;
}

void MaerklinMotorola::Parse() {
	for(int QueuePos=0; QueuePos<MM_QUEUE_LENGTH;QueuePos++) {
		if(DataGramState_ReadyToParse == DataQueue[QueuePos].State) {
		  int period = DataQueue[QueuePos].Timings[0]+DataQueue[QueuePos].Timings[1]; //bit-laenge berechnen
		  bool valid = true;
		  bool parsed = false;
		  
		  for(int i=0;i<35;i+=2) { //Bits dekodieren
			DataQueue[QueuePos].Bits[i/2] = (DataQueue[QueuePos].Timings[i]>(period>>1)) ? 1 : 0; //Länger als die Hälfte: 1

			if(i<33) {
			  int period_tmp = DataQueue[QueuePos].Timings[i] + DataQueue[QueuePos].Timings[i+1];
			  if(period_tmp > 125 && period_tmp < 175) valid = false; //MFX herausfiltern
			}
		  }

		  for(int i=0;i<9;i++) { //Trits aus Bits dekodieren
			DataQueue[QueuePos].Trits[i] = (DataQueue[QueuePos].Bits[i*2] == 1 && DataQueue[QueuePos].Bits[i*2+1] == 1) ? 1 : ((DataQueue[QueuePos].Bits[i*2] == 0 && DataQueue[QueuePos].Bits[i*2+1] == 0) ? 0 : 2);
		  }

		  //Decoder
		  if(DataQueue[QueuePos].tm_package_delta > 1300 && DataQueue[QueuePos].tm_package_delta < 4200 && valid) { //Protokollspezifische Telegramlänge: Weichen oder Lokprotokoll
			DataQueue[QueuePos].IsMagnet = ((period < 150) ? true : false);  //Unterscheidung Protokoll (Fest-Zeit)
			
			DataQueue[QueuePos].Address = DataQueue[QueuePos].Trits[3] * 27 + DataQueue[QueuePos].Trits[2] * 9 + DataQueue[QueuePos].Trits[1] * 3 + DataQueue[QueuePos].Trits[0];

			if(!DataQueue[QueuePos].IsMagnet) { //Loktelegramm
			  DataQueue[QueuePos].Function = (DataQueue[QueuePos].Trits[4] == 1) ? true : false;

			  int s = DataQueue[QueuePos].Bits[10] + DataQueue[QueuePos].Bits[12] * 2 + DataQueue[QueuePos].Bits[14] * 4 + DataQueue[QueuePos].Bits[16] * 8;
			  DataQueue[QueuePos].Stop = (s==0) ? true : false;
			  DataQueue[QueuePos].ChangeDir = (s==1) ? true : false;
			  DataQueue[QueuePos].Speed = (s==0) ?  0 : s-1;

			  parsed=true;
			} else { //Magnettelegramm
			  if(DataQueue[QueuePos].Trits[4]==0) {
				int s = DataQueue[QueuePos].Bits[10] + DataQueue[QueuePos].Bits[12] * 2 + DataQueue[QueuePos].Bits[14] * 4;
				DataQueue[QueuePos].SubAddress = s;
				DataQueue[QueuePos].MagnetState = (DataQueue[QueuePos].Bits[16]==1) ? true : false;
				
				parsed=true;
			  }
			}  
		  }	
		  if(parsed) {
			  //Get previous DataGram from Queue
			  int previousDataGramPos = QueuePos > 0 ? QueuePos-1 : MM_QUEUE_LENGTH - 1;
			  DataQueue[QueuePos].State = DataGramState_Parsed;
			  if(DataGramState_Parsed == DataQueue[previousDataGramPos].State) {
				  //Check if previous DataGram was identical
				  if(0 == memcmp(DataQueue[QueuePos].Trits, DataQueue[previousDataGramPos].Trits, 9)) {
					  DataQueue[QueuePos].State = DataGramState_Validated;
				  }
			  }
		  }
		  else {
			  //Invalid frame
			  DataQueue[QueuePos].State = DataGramState_Error;
		  }
		}
	}
}

void MaerklinMotorola::PinChange() {
  //bool state = digitalRead(pin);
  unsigned long tm = micros();
  unsigned long tm_delta = tm - last_tm;

  if(sync) { //erst nach syncronisation bits sammeln
    DataQueue[DataQueueWritePosition].Timings[timings_pos] = int(tm_delta); //ablage des zeitunterschieds zwischen den letzten flanken
    timings_pos++;

	if(tm_delta>500) {
		//timeout - resync
		timings_pos = 0;
		sync = true;
		sync_tm = tm;
	}
    if(timings_pos==35) {
      DataQueue[DataQueueWritePosition].tm_package_delta = tm - sync_tm; //paket-laenge berechen
	  DataQueue[DataQueueWritePosition].State = DataGramState_ReadyToParse;
	  DataQueueWritePosition ++;
	  //Queue end - go to queue start
	  if(MM_QUEUE_LENGTH <= DataQueueWritePosition)
	  {
		 DataQueueWritePosition = 0;
	  }
	  DataQueue[DataQueueWritePosition].State = DataGramState_Reading;
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
