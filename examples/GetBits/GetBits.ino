#include <MaerklinMotorola.h>


#define INPUT_PIN 2

volatile MaerklinMotorola mm(INPUT_PIN);

void setup() {
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), isr, CHANGE);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  if(mm.DataAvailable) {
    for(int i=0;i<18;i++) {
      Serial.print(mm.Data.Bits[i]);
    }

    Serial.println();

    mm.DataAvailable = false;
  }
}

void isr() {
  mm.PinChange();
}
