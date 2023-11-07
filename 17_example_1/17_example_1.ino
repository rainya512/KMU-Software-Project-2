// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3

void setup()
{
  Serial.begin(57600);
}

void loop()
{
  int a_value = analogRead(PIN_POTENTIOMETER);

  Serial.print("ADC Read: "); 
  Serial.print(a_value);
  Serial.print(" = ");
  Serial.print((a_value / 1024.0) * 5.0);
  Serial.println(" Volt");
  delay(100);
}

