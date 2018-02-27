/*
  MaerklinMotorola.h - Library for decoding the signals from the Märklin-Motorola-protocol. 
  Created by Michael Henzler (Laserlicht), Februar 27, 2018.
  Released under BSD 2-Clause "Simplified" License.
*/
#ifndef MaerklinMotorola_h
#define MaerklinMotorola_h

#include "Arduino.h"


struct MaerklinMotorolaData {
  byte Bits[18];
  byte Trits[9];
  int Timings[35];

  bool IsMagnet;
  
  int Address;
  
  bool Function;
  bool Stop;
  bool ChangeDir;
  int Speed;

  int SubAddress;
  bool MagnetState; //bei aus werden normalerweise alle ausgeschaltet
};

class MaerklinMotorola {
public:
  MaerklinMotorola(int p);
  void PinChange();

  bool DataAvailable;
  MaerklinMotorolaData Data;
private:
  int pin;
  unsigned long last_tm = 0;
  unsigned long sync_tm = 0;
  bool sync = false;
  int timings_pos = 0;
};

#endif
