#pragma once

#include <mbed.h>

class StepperMotor {
    public:
    StepperMotor(PinName enable_pin, PinName step_pin, PinName dir_pin, PinName ms1_pin, PinName ms2_pin, 
                 PinName ms3_pin, PinName sleep_pin);
    
    void step_positive();
    void step_negative();
    int32_t get_steps();
    void reset_step_count();

    private:
    DigitalOut enable;
    PwmOut step;
    DigitalOut dir;
    DigitalOut ms1;
    DigitalOut ms2;
    DigitalOut ms3;
    DigitalOut sleep;
    int32_t count;
};