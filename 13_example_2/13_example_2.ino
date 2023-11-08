#include <Servo.h>

// Arduino pin assignment
#define PIN_LED 9
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10

// configurable parameters for sonar
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

// Target Distance
#define _TARGET_LOW  180.0
#define _TARGET_HIGH 220.0

#define _DUTY_MIN 530 // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1480 // servo neutral position (90 degree)
#define _DUTY_MAX 2430 // servo full counter-clockwise position (180 degree)

#define _INTERVAL_DIST    10 // USS interval (unit: msec)
#define _INTERVAL_SERVO   20 // servo interval (unit: msec)
#define _INTERVAL_SERIAL  40 // serial interval (unit: msec)

#define TIMEOUT ((_INTERVAL_DIST / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

// global variables

Servo myservo;

unsigned long last_sampling_time_dist;   // unit: msec
unsigned long last_sampling_time_servo;  // unit: msec
unsigned long last_sampling_time_serial; // unit: msec

bool event_dist, event_servo, event_serial; // event triggered?

float  dist_prev = _DIST_MIN;   // Sonar range filter

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_TRIG,OUTPUT);
  digitalWrite(PIN_TRIG, LOW); 
  pinMode(PIN_ECHO,INPUT);

  digitalWrite(PIN_LED, 1);

  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  // initialize serial port
  Serial.begin(115200);  
}
  
void loop() {
  float  dist_raw;
  unsigned long time_curr = millis();
  
  // wait until next event time
  if (time_curr >= (last_sampling_time_dist + _INTERVAL_DIST)) {
    last_sampling_time_dist += _INTERVAL_DIST;
    event_dist = true;
  }
  if (time_curr >= (last_sampling_time_servo + _INTERVAL_SERVO)) {
    last_sampling_time_servo += _INTERVAL_SERVO;
    event_servo = true;
  }
  if (time_curr >= (last_sampling_time_serial + _INTERVAL_SERIAL)) {
    last_sampling_time_serial += _INTERVAL_SERIAL;
    event_serial = true;
  }
    
  if (event_dist) {
    event_dist = false;
    // get a distance reading from the USS
    dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);
    // Apply range filter
    if ((dist_raw < _DIST_MIN) || (dist_raw > _DIST_MAX))
        dist_raw = dist_prev;
    else
        dist_prev = dist_raw;
  }
  
  if (event_servo) {
    event_servo = false;
    // adjust servo position according to the USS read value
    if (dist_raw < _TARGET_LOW) {
      myservo.writeMicroseconds(_DUTY_MIN);
      digitalWrite(PIN_LED, 1);
    } else if (dist_raw > _TARGET_HIGH) {
      myservo.writeMicroseconds(_DUTY_MAX);
      digitalWrite(PIN_LED, 1);
    } else {
      myservo.writeMicroseconds(_DUTY_NEU);
      digitalWrite(PIN_LED, 0);
    }
  }
  
  if (event_serial) {
    event_serial = false;
    // output the read values to the serial port
    Serial.print("Min:");    Serial.print(_DIST_MIN);
    Serial.print(",Low:");   Serial.print(_TARGET_LOW);
    Serial.print(",raw:");   Serial.print(dist_raw);
    Serial.print(",servo:"); Serial.print(myservo.read());  
    Serial.print(",High:");  Serial.print(_TARGET_HIGH);
    Serial.print(",Max:");   Serial.print(_DIST_MAX);
    Serial.println("");
  }
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
