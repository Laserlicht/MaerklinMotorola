/*
  MaerklinMotorola.h - Library for decoding the signals from the Märklin-Motorola-protocol. 
  Created by Michael H. (Laserlicht), Februar 27, 2018.
  Released under BSD 2-Clause "Simplified" License.
*/

#ifndef MaerklinMotorola_h
#define MaerklinMotorola_h

#include "Arduino.h"

#define MM_QUEUE_LENGTH	10

enum DataGramState
{
	DataGramState_Reading,
	DataGramState_ReadyToParse,
	DataGramState_Parsed,
	DataGramState_Validated,
	DataGramState_Finished,
	DataGramState_Error,
};

enum MM2DirectionState
{
	MM2DirectionState_Unavailable,
	MM2DirectionState_Forward,
	MM2DirectionState_Backward,
};

enum MM2DecoderState
{
	MM2DecoderState_Unavailable,
	MM2DecoderState_Red,
	MM2DecoderState_Green,
};

struct MaerklinMotorolaData {
  byte Trits[9];
  int Timings[35];
  unsigned long tm_package_delta;
  MM2DirectionState MM2Direction;
  
  unsigned char Address;
  
  unsigned char Speed;
  unsigned char Step;
  unsigned char MM2FunctionIndex;

  unsigned char SubAddress;
  unsigned char PortAddress; // verbose "port" address (1 to 256 / 320)

  DataGramState State;
  
  bool Function;
  bool Stop;
  bool ChangeDir;
  bool MagnetState; //with off normally all are switched off
  MM2DecoderState DecoderState; // red (false) or green (true)
  bool IsMagnet;
  bool IsMM2;
  bool IsMM2FunctionOn;
};

class MaerklinMotorola {
public:
  MaerklinMotorola(int p);
  void PinChange();
  MaerklinMotorolaData* GetData();
  void Parse();

private:
  int pin;
  unsigned long last_tm = 0;
  unsigned long sync_tm = 0;
  bool sync = false;
  char timings_pos = 0;
  char DataQueueWritePosition = 0;
  MaerklinMotorolaData DataQueue[MM_QUEUE_LENGTH];
};

#endif
