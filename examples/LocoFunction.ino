#include <MaerklinMotorola.h>

#define INPUT_PIN 2

volatile MaerklinMotorola mm(INPUT_PIN);

void setup() {
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), isr, CHANGE);

  pinMode(LED_BUILTIN, OUTPUT);

  //Serial.begin(115200);
}

void loop() {
  if(mm.DataAvailable) {
    /*for(int i=0;i<9;i++) {
      Serial.print(mm.Data.Trits[i]);
    }*/
    
    /*Serial.print("Address: "); Serial.print(mm.Data.Address);
    Serial.print(" -  Function: "); Serial.print(mm.Data.Function);
    Serial.print(" -  Stop: "); Serial.print(mm.Data.Stop);
    Serial.print(" -  ChangeDir: "); Serial.print(mm.Data.ChangeDir);
    Serial.print(" -  Speed: "); Serial.print(mm.Data.Speed);
    Serial.print(" -  Magnet: " + String(mm.Data.IsMagnet ? "yes" : "no"));
    
    Serial.println();*/

    if(!mm.Data.IsMagnet && mm.Data.Address == 24) {
      digitalWrite(LED_BUILTIN, mm.Data.Function);
    }

    mm.DataAvailable = false;
  }
}

void isr() {
  mm.PinChange();
}
