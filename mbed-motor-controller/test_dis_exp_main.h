#include <cstdint>
#include <mbed.h>
#include "dis_exp.h"

const PinName MOTOR_ENABLE = D15;
const PinName MOTOR_STEP = D9;
const PinName MOTOR_DIR = D13;
const PinName MOTOR_MS1 = D12;
const PinName MOTOR_MS2 = D11;
const PinName MOTOR_MS3 = D10;
const PinName SLEEP = D1;

const PinName CS_MEASURE = D2;
const PinName CS_SQUARE = D3;

const int BAUD_RATE = 9600;

const int32_t N_MEASURE = 4;
const uint32_t T_MEASURE_US = 400; //currently not in use
const int32_t DELTA_STEPS = 20;
const int32_t MAX_STEPS = 100;

int main(void) {
    StepperMotor motor(MOTOR_ENABLE, 
                       MOTOR_STEP, 
                       MOTOR_DIR, 
                       MOTOR_MS1,
                       MOTOR_MS2, 
                       MOTOR_MS3,
                       SLEEP);
    
    CapSense cap_sense(CS_MEASURE, CS_SQUARE);

    DiscreteExperiment discrete_exp(motor, cap_sense, N_MEASURE, T_MEASURE_US,
                                    DELTA_STEPS, MAX_STEPS);
    
    discrete_exp.reset();

    discrete_exp.start();

    int steps = 0;
    while(!discrete_exp.is_done()) {
        printf("Steps: %d\n", steps);
        steps++;
        discrete_exp.proceed();
    }

    auto& data = discrete_exp.report();

    for (auto& steps_tau_tuple: data) {
        printf("Steps: %d, Tau: %d\n",
               std::get<0>(steps_tau_tuple),
               std::get<1>(steps_tau_tuple));
    }
}