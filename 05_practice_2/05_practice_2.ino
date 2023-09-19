#define PIN_LED 7

int toggle;
int i;

int toggleState(int state) {
  if(!state) return 1;
  
  return 0;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  toggle = 0;
  i = 0;
}

void loop() {
  Serial.println("on");
  digitalWrite(PIN_LED, 1); //turn on LED
  delay(1000); //wait for 1 second

  for(i; i < 10; i++) { //repeat 10 times for 1 second
    digitalWrite(PIN_LED, toggle);
    toggle = toggleState(toggle);
    Serial.println(toggle);
    delay(100);
  }

  digitalWrite(PIN_LED, 0); //turn off LED
  Serial.println("end");
  while(1) { //infinite loop

  }
}
