# Arduino PID Speed Controller & Thruster Safety Cutoff

## Overview
A simulated PID-based speed controller for a stepper motor with 
thruster safety cutoff logic, designed with ROV applications in mind.

## Tech Stack
Arduino Uno | A4988 Stepper Driver | Embedded C | Wokwi Simulation

## How It Works
- PID loop continuously adjusts PWM output to maintain target speed
- Safety cutoff triggers when speed exceeds threshold, 
  simulating thruster protection in underwater robotics
- Tuned Kp, Ki, Kd values for stable motor response

## Simulation
🔗 [Run on Wokwi Thruster Safety Cutoff ](<(https://wokwi.com/projects/460842084391044097)>)
🔗 [Run on Wokwi Thruster PID-based speed controller ](<(https://wokwi.com/projects/460842063649160193)>)

## How to Run Locally
1. Open Arduino IDE
2. Load .ino files
3. Upload to Arduino Uno
