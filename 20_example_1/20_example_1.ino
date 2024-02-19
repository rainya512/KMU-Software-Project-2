// Arduino pin assignment
#define PIN_IR A0

void setup()
{
  Serial.begin(1000000);  // initialize serial port
}

void loop()
{
  unsigned int filtered; // Voltage values from the IR sensor (0 ~ 1023)

  while (Serial.available() == 0)
    ;
  Serial.read();
  
  // Take a median value from multiple measurements
  filtered = ir_sensor_filtered(100, 0.5, 0); // Replace n with your desired value
  Serial.print("FLT:"); Serial.println(filtered);
  //while (1) ;
}

int compare(const void *a, const void *b) {
  return (*(unsigned int *)a - *(unsigned int *)b);
}

unsigned int ir_sensor_filtered(unsigned int n, float position, int verbose)
{
  // Eliminate spiky noise of an IR distance sensor by repeating measurement and taking a middle value
  // n: number of measurement repetition
  // position: the percentile of the sample to be taken (0.0 <= position <= 1.0)
  // verbose: 0 - normal operation, 1 - observing the internal procedures, and 2 - calculating elapsed time.
  // Example 1: ir_sensor_filtered(n, 0.5, 0) => return the median value among the n measured samples.
  // Example 2: ir_sensor_filtered(n, 0.0, 0) => return the smallest value among the n measured samples.
  // Example 3: ir_sensor_filtered(n, 1.0, 0) => return the largest value among the n measured samples.

  // The output of Sharp infrared sensor includes lots of spiky noise.
  // To eliminate such a spike, ir_sensor_filtered() performs the following two steps:
  // Step 1. Repeat measurement n times and collect n * position smallest samples, where 0 <= postion <= 1.
  // Step 2. Return the position'th sample after sorting the collected samples.

  // returns 0, if any error occurs

  unsigned int *ir_val, ret_val;
  unsigned int start_time;
 
  if (verbose >= 2)
    start_time = millis(); 

  if ((n == 0) || (n > 100) || (position < 0.0) || (position > 1))
    return 0;
    
  if (position == 1.0)
    position = 0.999;

  if (verbose == 1) {
    Serial.print("n: "); Serial.print(n);
    Serial.print(", position: "); Serial.print(position); 
    Serial.print(", ret_idx: ");  Serial.println((unsigned int)(n * position)); 
  }

  ir_val = (unsigned int *)malloc(sizeof(unsigned int) * n);
  if (ir_val == NULL)
    return 0;

  if (verbose == 1)
    Serial.print("IR:");
  
  for (int i = 0; i < n; i++) {
    ir_val[i] = analogRead(PIN_IR);
    if (verbose == 1) {
        Serial.print(" ");
        Serial.print(ir_val[i]);
    }
  }

  if (verbose == 1)
    Serial.print  ("  => ");

  qsort(ir_val, n, sizeof(unsigned int), compare);
  ret_val = ir_val[(unsigned int)(n * position)];

  if (verbose == 1) {
    for (int i = 0; i < n; i++) {
        Serial.print(" ");
        Serial.print(ir_val[i]);
    }
    Serial.print(" :: ");
    Serial.println(ret_val);
  }
  free(ir_val);

  if (verbose >= 2) {
    Serial.print("Elapsed time: "); Serial.print(millis() - start_time); Serial.println("ms");
  }
  
  return ret_val;
}
