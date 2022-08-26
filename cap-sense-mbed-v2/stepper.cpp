#include <cstdio>
#include <mbed.h>
#include "stepper.h"
#include <chrono>
//#include <thread>

StepperMotor::StepperMotor(PinName enable_pin, 
                           PinName step_pin, 
                           PinName dir_pin, 
                           PinName ms1_pin, 
                           PinName ms2_pin, 
                           PinName ms3_pin,
                           PinName sleep_pin)
    : enable(enable_pin), step(step_pin), dir(dir_pin), ms1(ms1_pin),
      ms2(ms2_pin), ms3(ms3_pin), sleep(sleep_pin), count(0) {
        enable = 0;
        sleep = 1;
        step = 0;
    }

void StepperMotor::step_positive() {
    dir = 0;
    step = 1;
    
    wait_us(40000);
    if (step == 1) {
        printf("step rise\n");
    };
    if (step == 0){
        printf("step fall, also something is wrong\n");
    };
    
    step = 0;

    wait_us(40000);
    printf("one step taken\n");
}

void StepperMotor::step_negative() {
    dir = 1;
    step = 1;
    wait_us(40);
    step = 0;
    wait_us(40);
}

int32_t StepperMotor::get_steps() {
    return count;
}

void StepperMotor::reset_step_count() {
    count = 0;
}
