#include <MaerklinMotorola.h>


#define INPUT_PIN 2

volatile MaerklinMotorola mm(INPUT_PIN);

void setup() {
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), isr, CHANGE);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  mm.Parse();
  MaerklinMotorolaData* Data = mm.GetData();
  if(Data) {
    for(int i=0;i<9;i++) {
      Serial.print(Data->Trits[i]);
    }

    Serial.println();
  }
}

void isr() {
  mm.PinChange();
}
