#include <MaerklinMotorola.h>

#define INPUT_PIN 2

volatile MaerklinMotorola mm(INPUT_PIN);

void setup() {
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), isr, CHANGE);

  pinMode(LED_BUILTIN, OUTPUT);

  //Serial.begin(115200);
}

void loop() {
  mm.Parse();
  MaerklinMotorolaData* Data = mm.GetData();
  if(Data) {
    /*for(int i=0;i<9;i++) {
      Serial.print(Data->Trits[i]);
    }*/
    
    /*Serial.print("Address: "); Serial.print(Data->Address);
    Serial.print(" -  Function: "); Serial.print(Data->Function);
    Serial.print(" -  Stop: "); Serial.print(Data->Stop);
    Serial.print(" -  ChangeDir: "); Serial.print(Data->ChangeDir);
    Serial.print(" -  Speed: "); Serial.print(Data->Speed);
    Serial.print(" -  Magnet: " + String(Data->IsMagnet ? "yes" : "no"));
    
    Serial.println();*/

    if(!Data->IsMagnet && Data->Address == 24) {
      digitalWrite(LED_BUILTIN, Data->Function);
    }
  }
}

void isr() {
  mm.PinChange();
}
