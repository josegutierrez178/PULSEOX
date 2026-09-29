// importing libraries
#include <Arduino.h>
#include <limits.h>  
#include <math.h>    

// Setting hardware pins 
const int RED_LED = 12; // digital pin for Red LED
const int IR_LED  = 11; // digital pin for IR LED
const int ALS_PIN = A0; // analog pin for ambient light sensor

// Sampling parameters
const int MAX_SAMPLES  = 50; // maximum number of samples per period
const int MAX_PERIODS = 12; //maximum number of periods
const int SAMPLE_SIZE  = 4;   // minimum valid peaks/valleys before analysis
const int WINDOW = 6;       // peak/valley search window 

// Buffers for averaged samples
int  redValues[MAX_SAMPLES]; // hold averaged readings
int  irValues[MAX_SAMPLES]; // hold averaged readings
unsigned long timeValues[MAX_SAMPLES]; // store timestamps for each averaged reading

int  sampleCount = 0;        // initial count
bool enoughData  = false;    // true once there are at least 4 peaks/valleys

// Function declarations 
void acquireSensorData();      // gather sensor samples from LEDs
void validateSamples();        // test if we have enough valid data
void runProcessingAndAnalysis(); // analyze data for SPO2

// Helper functions for peak/valley logic 
/*
*Finds the maximum value in a given section of an array
*Takes an array, the start index and the end index as the parameters
*Returns the maximum value in that section of the array
*/
int maxInRange(const int *arr, int start, int end) {
  if (start > end) return INT_MIN;
  int m = arr[start];
  for (int i = start + 1; i <= end; ++i) if (arr[i] > m) m = arr[i];
  return m;
}
/*
*Finds the minimum value in a given section of an array
*Takes an array, the start index and the end index as the parameters
*Returns the minimum value in that section of the array
*/
int minInRange(const int *arr, int start, int end) {
  if (start > end) return INT_MAX;
  int m = arr[start];
  for (int i = start + 1; i <= end; ++i) if (arr[i] < m) m = arr[i];
  return m;
}
/*
*Calculates the median of values in an array
*Takes an array and the number of values in the array whose median it will calculate
*Returns the median
*/
double medianOf(const int *arr, int n) {
  if (n <= 0) return 0.0;
  // temporary array with size MAX_SAMPLES
  int tmp[MAX_SAMPLES];
  for (int i = 0; i < n; ++i) tmp[i] = arr[i];
  // sorting algorithm to figure out the middle value
  for (int i = 1; i < n; ++i) {
    int key = tmp[i];
    int j = i - 1;
    while (j >= 0 && tmp[j] > key) { tmp[j+1] = tmp[j]; j--; }
    tmp[j+1] = key;
  }
  if (n % 2 == 1) return (double)tmp[n/2];
  return (tmp[n/2 - 1] + tmp[n/2]) / 2.0;
}

// Arduino setup
void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(IR_LED,  OUTPUT);
  Serial.begin(9600);              // allow serial monitor output
  digitalWrite(RED_LED, LOW);      // start off
  digitalWrite(IR_LED,  LOW);      // start off
  delay(50);                       // short delay before sampling
}

// Main repeating loop
void loop() {
  acquireSensorData();  // collect new averaged readings
  validateSamples();    // check for enough peaks/valleys

  // once enough samples are stored, trigger processing & analysis
  if (enoughData) {
    runProcessingAndAnalysis();    // compute SPO2
    enoughData  = false;           // reset so we analyze again when new data exists
    sampleCount = 0;               // reset window
  }
}

// Acquire averaged RED/IR readings
void acquireSensorData() {
  // Red LED sampling over 20ms and averaging the readings taken during 20ms
  digitalWrite(RED_LED, HIGH);
  unsigned long startTime = millis();
  long redSum = 0;
  int  redSamples = 0;
  while (millis() - startTime < 20) {  // sample for ~20 ms
    redSum += analogRead(ALS_PIN);
    redSamples++;
  }
  digitalWrite(RED_LED, LOW);
  int redAvg = (redSamples > 0) ? (redSum / redSamples) : 0;

  // IR LED sampling over 20ms and averaging the readings taken during 20ms
  digitalWrite(IR_LED, HIGH);
  startTime = millis();
  long irSum = 0;
  int  irSamples = 0;
  while (millis() - startTime < 20) {
    irSum += analogRead(ALS_PIN);
    irSamples++;
  }
  digitalWrite(IR_LED, LOW);
  int irAvg = (irSamples > 0) ? (irSum / irSamples) : 0;

  // Store averaged samples in circular buffer 
  int idx = sampleCount % MAX_SAMPLES;
  redValues[idx] = redAvg;
  irValues [idx] = irAvg;
  timeValues[idx] = millis();
  sampleCount++;

  // immediate output for Python plotting
  Serial.print("RED,");
  Serial.print(redAvg);
  Serial.print(",IR,");
  Serial.print(irAvg);
  Serial.print(",t,");
  Serial.println(timeValues[idx]);
}

// Validate that we have enough peaks/valleys 
void validateSamples() {
  int n = (sampleCount < MAX_SAMPLES) ? sampleCount : MAX_SAMPLES; //making sure we only copy the filled array positions
  if (n < 3) { enoughData = false; return; }

  // create chronological copies of RED and IR samples
  int snapshot[MAX_SAMPLES];
  int snapshotIR[MAX_SAMPLES];
  int startIndex = (sampleCount <= MAX_SAMPLES) ? 0 : (sampleCount % MAX_SAMPLES);

  if (sampleCount <= MAX_SAMPLES) {
    for (int i = 0; i < n; ++i) {
      snapshot[i]  = redValues[i];
      snapshotIR[i]= irValues[i];
    }
  } else {
    for (int i = 0; i < n; ++i) {
      int idx = (startIndex + i) % MAX_SAMPLES;
      snapshot[i]   = redValues[idx];
      snapshotIR[i] = irValues[idx];
    }
  }

  // count peaks/valleys by comparing to adjacent values to validate that there are st least 4 peaks and valleys
  int redPeaks = 0, redValleys = 0;
  for (int i = 1; i < n - 1; i++) {
    if (snapshot[i]  > snapshot[i - 1] && snapshot[i]  > snapshot[i + 1]) redPeaks++;
    if (snapshot[i]  < snapshot[i - 1] && snapshot[i]  < snapshot[i + 1]) redValleys++;
  }

  int irPeaks = 0, irValleys = 0;
  for (int i = 1; i < n - 1; i++) {
    if (snapshotIR[i] > snapshotIR[i - 1] && snapshotIR[i] > snapshotIR[i + 1]) irPeaks++;
    if (snapshotIR[i] < snapshotIR[i - 1] && snapshotIR[i] < snapshotIR[i + 1]) irValleys++;
  }

  if (redPeaks >= SAMPLE_SIZE && redValleys >= SAMPLE_SIZE &&
      irPeaks  >= SAMPLE_SIZE && irValleys  >= SAMPLE_SIZE)
    enoughData = true;
  else
    enoughData = false;
}

/*
*Determines that there are at least 4 peaks and values
*Determines the peaks and valleys and their timestamps
*Calculates SpO2 and BPM
*/
void runProcessingAndAnalysis() {
  int n = (sampleCount < MAX_SAMPLES) ? sampleCount : MAX_SAMPLES;
  if (n < 3) {
    Serial.println("Not enough samples to analyze");
    return;
  }

  // construct chronological snapshots with timestamps to use to get peak and valley values
  int snapshot[MAX_SAMPLES];
  int snapshotIR[MAX_SAMPLES];
  unsigned long snapshotTimes[MAX_SAMPLES];

  int startIndex = (sampleCount <= MAX_SAMPLES) ? 0 : (sampleCount % MAX_SAMPLES);
  if (sampleCount <= MAX_SAMPLES) {
    for (int i = 0; i < n; ++i) {
      snapshot[i]   = redValues[i];
      snapshotIR[i] = irValues[i];
      snapshotTimes[i] = timeValues[i];
    }
  } else {
    for (int i = 0; i < n; ++i) {
      int idx = (startIndex + i) % MAX_SAMPLES;
      snapshot[i]   = redValues[idx];
      snapshotIR[i] = irValues[idx];
      snapshotTimes[i] = timeValues[idx];
    }
  }

  // find peaks/valleys using the window and median threshold for middle region
  int redPeaksVals[MAX_SAMPLES];
  int redValleysVals[MAX_SAMPLES];
  int irPeaksVals[MAX_SAMPLES];
  int irValleysVals[MAX_SAMPLES];
  unsigned long irValleyTimes[MAX_SAMPLES];

  int rp = 0, rv = 0, ip = 0, iv = 0;

  double medRed = medianOf(snapshot, n);
  double medIR  = medianOf(snapshotIR, n);

  for (int i = 1; i < n - 1 && (rp < MAX_SAMPLES || rv < MAX_SAMPLES || ip < MAX_SAMPLES || iv <  MAX_SAMPLES); ++i) {
    // Storing RED peaks
    bool isRPeak = false;
    if (i < WINDOW) {
      int leftMax  = (i-1 >= 0) ? maxInRange(snapshot, 0, i-1) : INT_MIN;
      int rightMax = maxInRange(snapshot, i+1, min(n-1, i + WINDOW));
      if (snapshot[i] > leftMax && snapshot[i] > rightMax) isRPeak = true;
    } else if (i >= WINDOW && i < n - WINDOW) {
      int leftMax  = maxInRange(snapshot, i - WINDOW, i - 1);
      int rightMax = maxInRange(snapshot, i + 1, i + WINDOW);
      if (leftMax < snapshot[i] && rightMax < snapshot[i] && snapshot[i] > medRed) isRPeak = true;
    } else {
      int leftMax  = maxInRange(snapshot, max(0, i - WINDOW), i - 1);
      int rightMax = (i+1 <= n-1) ? maxInRange(snapshot, i + 1, n - 1) : INT_MIN;
      if (snapshot[i] > leftMax && snapshot[i] > rightMax) isRPeak = true;
    }
    if (isRPeak && rp < SAMPLE_SIZE) {
      redPeaksVals[rp++] = snapshot[i];
    }

    // Storing RED valleys 
    bool isRValley = false;
    if (i >= WINDOW && i < n - WINDOW) {
      int leftMin  = minInRange(snapshot, i - WINDOW, i - 1);
      int rightMin = minInRange(snapshot, i + 1, i + WINDOW);
      if (leftMin > snapshot[i] && rightMin > snapshot[i] && snapshot[i] < medRed) isRValley = true;
    } else {
      int leftMin  = (i-1 >= 0) ? minInRange(snapshot, max(0, i - WINDOW), i - 1) : INT_MAX;
      int rightMin = (i+1 <= n-1) ? minInRange(snapshot, i + 1, n - 1) : INT_MAX;
      if (snapshot[i] < leftMin && snapshot[i] < rightMin) isRValley = true;
    }
    if (isRValley && rv < SAMPLE_SIZE) {
      redValleysVals[rv++] = snapshot[i];
    }

    // Storing IR peaks
    bool isIPeak = false;
    if (i < WINDOW) {
      int leftMax  = (i-1 >= 0) ? maxInRange(snapshotIR, 0, i-1) : INT_MIN;
      int rightMax = maxInRange(snapshotIR, i+1, min(n-1, i + WINDOW));
      if (snapshotIR[i] > leftMax && snapshotIR[i] > rightMax) isIPeak = true;
    } else if (i >= WINDOW && i < n - WINDOW) {
      int leftMax  = maxInRange(snapshotIR, i - WINDOW, i - 1);
      int rightMax = maxInRange(snapshotIR, i + 1, i + WINDOW);
      if (leftMax < snapshotIR[i] && rightMax < snapshotIR[i] && snapshotIR[i] > medIR) isIPeak = true;
    } else {
      int leftMax  = maxInRange(snapshotIR, max(0, i - WINDOW), i - 1);
      int rightMax = (i+1 <= n-1) ? maxInRange(snapshotIR, i + 1, n - 1) : INT_MIN;
      if (snapshotIR[i] > leftMax && snapshotIR[i] > rightMax) isIPeak = true;
    }
    if (isIPeak && ip < SAMPLE_SIZE) {
      irPeaksVals[ip++] = snapshotIR[i];
    }

    // Storing IR valleys
    bool isIValley = false;
    if (i >= WINDOW && i < n - WINDOW) {
      int leftMin  = minInRange(snapshotIR, i - WINDOW, i - 1);
      int rightMin = minInRange(snapshotIR, i + 1, i + WINDOW);
      if (leftMin > snapshotIR[i] && rightMin > snapshotIR[i] && snapshotIR[i] < medIR) isIValley = true;
    } else {
      int leftMin  = (i-1 >= 0) ? minInRange(snapshotIR, max(0, i - WINDOW), i - 1) : INT_MAX;
      int rightMin = (i+1 <= n-1) ? minInRange(snapshotIR, i + 1, n - 1) : INT_MAX;
      if (snapshotIR[i] < leftMin && snapshotIR[i] < rightMin) isIValley = true;
    }
    if (isIValley && iv < SAMPLE_SIZE) {
      irValleysVals[iv] = snapshotIR[i];
      irValleyTimes[iv] = snapshotTimes[i];
      iv++;
    }
  } 

  
  int pairs = min(min(rp, rv), min(ip, iv)); //finds how many complete, valid pairs of red and IR peaks and valleys you have
  //if (pairs <= 0) {
    //Serial.println("No usable peak/valley pairs found"); debugging
    //return;
  //}

  double sumR = 0.0; // initializes at zero
  int usedPairs = 0; //initializes pairs for red and ir valley/peaks 
  for (int i = 0; i < pairs; ++i) { // loops over each of the Ir and Red valley pairs 
    double red_valley = (double)redValleysVals[i];
    double ir_valley  = (double)irValleysVals[i];
    if (red_valley <= 0.0 || ir_valley <= 0.0) continue; 
    double red_ac = (double)(redPeaksVals[i] - redValleysVals[i]); 
    double ir_ac  = (double)(irPeaksVals[i] - irValleysVals[i]); 
    if (ir_ac <= 0.0) continue; 
    double numerator   = red_ac / red_valley; //creates numerator 
    double denominator = ir_ac  / ir_valley; // creates denominator
    if (denominator <= 0.0) continue; 
    double Rls = numerator / denominator; 
    if (!isfinite(Rls)) continue; //makes sure it is a real number 
    if (Rls < 0.4) Rls = 0.4; // bound it to what would be normal for a human being
    if (Rls > 1.2) Rls = 1.2; 
    sumR += Rls;
    usedPairs++;
  }
  if (usedPairs == 0) { //another check
    Serial.println("Pairs invalid (division by zero / bad)");
    return;
  }

  // SPO2 computation. Once avg R is found, then its calculated. 
  double R = sumR / (double)usedPairs;
  double SPO2 = 110.0 - 25.0 * R; // required formula
  SPO2 += 10.0;                   // calibration offset (+10)
  if (SPO2 < 0.0) SPO2 = 0.0; 
  if (SPO2 > 100.0) SPO2 = 100.0; 

  // output result & BPM (using IR valley timestamps)
  Serial.print("Computed SPO2, "); //tells it to print sp02
  Serial.println(SPO2, 2); 

  // BPM Calculation, uses period avg (in ms) to compute beats per second, ir valley times. 
  if (iv > 1) {
    unsigned long totalDiff = 0;
    for (int i = 0; i < iv - 1; ++i) {
      if (irValleyTimes[i] != 0 && irValleyTimes[i+1] != 0){ //using IR valley times, it caculates an average period
        totalDiff += (irValleyTimes[i + 1] - irValleyTimes[i]);
      }
    }
    double avgPeriod = totalDiff / (double)(iv - 1); // in ms per beat
    if (avgPeriod > 0.0) {
      double BPM = (60000.0 / avgPeriod); //uses average period to see how many cycles happen in a minute/ finds bpm
      Serial.print("BPM,");
      Serial.println(BPM, 1);
    //} else {
      //Serial.println(",BPM: --"); if no bpm
    }
  } //else {
    //Serial.println(" | BPM: --"); if no bpm
  //}
}
