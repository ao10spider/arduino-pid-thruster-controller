// ============================================================
// Task 07 - Part 1: PID Speed Controller (Stepper + A4988)
// Author: Anton Ouseph
// Platform: Arduino Uno + A4988 + Stepper Motor (Wokwi)
// ============================================================

#define STEP_PIN   3    // PWM pin → A4988 STEP
#define DIR_PIN    4    // A4988 DIR
#define ENABLE_PIN 8    // A4988 ENABLE (LOW = enabled)

//PID Constants
const float Kp = 1.2;
const float Ki = 0.05;
const float Kd = 0.8;

//State
float targetRPM  = 200.0;
float currentRPM = 0.0;
float prevError  = 0.0;
float integral   = 0.0;

const int STEPS_PER_REV = 200;  

void setup() {
  Serial.begin(9600);
  pinMode(STEP_PIN,   OUTPUT);
  pinMode(DIR_PIN,    OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);   // enable the driver
  digitalWrite(DIR_PIN,    HIGH);  // set direction
  Serial.println("=== PID Speed Controller (Stepper) ===");
  Serial.println("Send target RPM via Serial (e.g. 200)");
  Serial.println("--------------------------------------");
}

void stepMotor(int steps, int delayUs) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(delayUs);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(delayUs);
  }
}

void loop() {
  // Read new target from Serial
  if (Serial.available() > 0) {
    float newTarget = Serial.parseFloat();
    if (newTarget > 0) {
      targetRPM = newTarget;
      integral  = 0.0;
      prevError = 0.0;
      Serial.print(">> New Target RPM: ");
      Serial.println(targetRPM);
    }
  }

  // PID Calculation 
  float error = targetRPM - currentRPM;

  float P = Kp * error;

  integral = constrain(integral + error, -500, 500);
  float I = Ki * integral;

  float D = Kd * (error - prevError);
  prevError = error;

  float output = constrain(P + I + D, 0, 255);

  //Convert output to step speed
  // output 0-255 maps to RPM 0-300
  float targetStepRPM = (output / 255.0) * 300.0;
  int   stepsToRun    = 10;
  int   stepDelay     = (targetStepRPM > 0)
                        ? (int)(60000000.0 / (targetStepRPM * STEPS_PER_REV * 2))
                        : 5000;
  stepDelay = constrain(stepDelay, 500, 10000);

  //Run motor steps
  stepMotor(stepsToRun, stepDelay);

  //Simulate current RPM with noise
  float noise = random(-8, 8);
  currentRPM += (output * 0.5 + noise - currentRPM * 0.03);
  currentRPM  = constrain(currentRPM, 0, 400);

  //Serial output
  Serial.print("Target: ");   Serial.print((int)targetRPM);
  Serial.print(" RPM | Current: "); Serial.print((int)currentRPM);
  Serial.print(" | Error: ");  Serial.print((int)error);
  Serial.print(" | P: ");      Serial.print(P, 1);
  Serial.print(" I: ");        Serial.print(I, 1);
  Serial.print(" D: ");        Serial.print(D, 1);
  Serial.print(" | Output: "); Serial.println((int)output);

  delay(300);
}
