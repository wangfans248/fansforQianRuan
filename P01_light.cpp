#include <Arduino.h>

 #define PIN_FLASHLIGHT 4

 void setup() {
   pinMode(PIN_FLASHLIGHT, OUTPUT);
 }

 void loop() {
   digitalWrite(PIN_FLASHLIGHT, HIGH);
   delay(1000);
   digitalWrite(PIN_FLASHLIGHT, LOW);
   delay(1000);
}
