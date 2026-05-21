// ============================================================
// Task 07 - Part 2: Thruster Safety Cutoff Logic
// Author: Anton Ouseph
// Platform: Arduino (Wokwi Simulator)
// ============================================================
// SAFETY RULES CHECKED:
//   Rule 1 — No two ADJACENT thrusters differ by more than 80
//             (prevents sudden mechanical jerk)
//   Rule 2 — No individual thruster value exceeds 230
//             (prevents overheating)
//   Rule 3 — Total PWM across all 6 thrusters must not exceed 900
//             (prevents power overload)
//
// If any rule is violated:
//   - The violation is printed to Serial
//   - Values are auto-clamped to a safe state
// ============================================================

#define NUM_THRUSTERS     6
#define MAX_SINGLE        230   // Max PWM per thruster (overheat limit)
#define MAX_TOTAL         900   // Max total power across all thrusters
#define MAX_ADJACENT_DIFF 80    // Max difference between adjacent thrusters

// ============================================================
// checkAndClamp()
//   Takes an array of 6 PWM values, checks all safety rules,
//   prints violations, and clamps to safe values in-place.
// ============================================================
void checkAndClamp(int pwm[], int n) {

  bool violated = false;

  Serial.println("--- Safety Check ---");

  //Rule 1: Adjacent Thruster Jerk Check
  for (int i = 0; i < n - 1; i++) {
    int diff = abs(pwm[i] - pwm[i + 1]);
    if (diff > MAX_ADJACENT_DIFF) {
      Serial.print("VIOLATION: T");
      Serial.print(i + 1);
      Serial.print(" and T");
      Serial.print(i + 2);
      Serial.print(" differ by ");
      Serial.print(diff);
      Serial.println(" (exceeds jerk limit of 80)");
      violated = true;
    }
  }

  //Rule 2: Single Thruster Overheat Check
  for (int i = 0; i < n; i++) {
    if (pwm[i] > MAX_SINGLE) {
      Serial.print("VIOLATION: T");
      Serial.print(i + 1);
      Serial.print(" exceeds max (");
      Serial.print(pwm[i]);
      Serial.println(" > 230)");
      violated = true;
    }
  }

  //Rule 3: Total Power Check
  int total = 0;
  for (int i = 0; i < n; i++) total += pwm[i];
  if (total > MAX_TOTAL) {
    Serial.print("VIOLATION: Total power ");
    Serial.print(total);
    Serial.println(" > 900");
    violated = true;
  }

  //Auto-Clamp to Safe State
  if (violated) {

    // Step 1: Clamp each value to MAX_SINGLE
    for (int i = 0; i < n; i++) {
      pwm[i] = constrain(pwm[i], 0, MAX_SINGLE);
    }

    // Step 2: Scale down proportionally if total still exceeds MAX_TOTAL
    total = 0;
    for (int i = 0; i < n; i++) total += pwm[i];
    if (total > MAX_TOTAL) {
      float scale = (float)MAX_TOTAL / total;
      for (int i = 0; i < n; i++) {
        pwm[i] = (int)(pwm[i] * scale);
      }
    }

    // Step 3: Smooth adjacent differences
    // Iteratively reduce differences that still exceed the jerk limit
    for (int pass = 0; pass < 5; pass++) {
      for (int i = 0; i < n - 1; i++) {
        int diff = abs(pwm[i] - pwm[i + 1]);
        if (diff > MAX_ADJACENT_DIFF) {
          int avg = (pwm[i] + pwm[i + 1]) / 2;
          pwm[i]     = avg + MAX_ADJACENT_DIFF / 2;
          pwm[i + 1] = avg - MAX_ADJACENT_DIFF / 2;
          // Re-clamp after smoothing
          pwm[i]     = constrain(pwm[i], 0, MAX_SINGLE);
          pwm[i + 1] = constrain(pwm[i + 1], 0, MAX_SINGLE);
        }
      }
    }

    // Print clamped values
    Serial.print("Clamped: [");
    for (int i = 0; i < n; i++) {
      Serial.print(pwm[i]);
      if (i < n - 1) Serial.print(", ");
    }
    Serial.println("]");

  } else {
    Serial.println("All checks PASSED. No violations.");
  }

  Serial.println("--------------------");
}

// ============================================================
void setup() {
  Serial.begin(9600);
  Serial.println("=== Thruster Safety Cutoff System ===");
  Serial.println();

  //Test Case 1: From problem statement
  Serial.println("Test 1: [120, 200, 80, 255, 90, 180]");
  int t1[NUM_THRUSTERS] = {120, 200, 80, 255, 90, 180};
  checkAndClamp(t1, NUM_THRUSTERS);
  Serial.println();

  //Test Case 2: All within safe limits
  Serial.println("Test 2: [100, 150, 130, 110, 120, 140]");
  int t2[NUM_THRUSTERS] = {100, 150, 130, 110, 120, 140};
  checkAndClamp(t2, NUM_THRUSTERS);
  Serial.println();

  //Test Case 3: Total power violation
  Serial.println("Test 3: [200, 210, 190, 180, 200, 220]");
  int t3[NUM_THRUSTERS] = {200, 210, 190, 180, 200, 220};
  checkAndClamp(t3, NUM_THRUSTERS);
  Serial.println();

  //Test Case 4: Jerk only
  Serial.println("Test 4: [50, 50, 50, 200, 50, 50]");
  int t4[NUM_THRUSTERS] = {50, 50, 50, 200, 50, 50};
  checkAndClamp(t4, NUM_THRUSTERS);
}

// ============================================================
void loop() {
  // All logic runs once in setup() for clean demo output
  // In a real system this would run continuously
}
