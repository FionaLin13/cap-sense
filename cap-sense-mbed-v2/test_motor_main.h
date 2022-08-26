#include <cstdio>
#include <mbed.h>
#include "stepper.h"

const PinName MOTOR_ENABLE = D15;
const PinName MOTOR_STEP = D9;
const PinName MOTOR_DIR = D13;
const PinName MOTOR_MS1 = D12;
const PinName MOTOR_MS2 = D11;
const PinName MOTOR_MS3 = D10;
const PinName SLEEP = D1;



int main() {
    StepperMotor motor(MOTOR_ENABLE, 
    MOTOR_STEP, 
    MOTOR_DIR, 
    MOTOR_MS1,
    MOTOR_MS2, 
    MOTOR_MS3,
    SLEEP);
    

  while(1) {
    motor.step_positive();
  }
}