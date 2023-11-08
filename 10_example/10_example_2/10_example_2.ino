#include <Servo.h>
#define PIN_SERVO 10

Servo myservo;

void setup() {
  myservo.attach(PIN_SERVO); 
  myservo.write(0);
  delay(500);
}

void loop() {
  // add code here.
  myservo.write(90);
  delay(500);
  myservo.write(180);
  delay(500);
  myservo.write(90);
  delay(500);
}
