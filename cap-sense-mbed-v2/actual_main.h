#include <cmath>
#include <cstdint>
#include <mbed.h>
#include "stepper.h"
#include "capsense.h"
#include "dis_exp.h"
#include "data_dumper.h"

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

const int32_t DIS_N_MEASURE = 20;
const uint32_t DIS_T_MEASURE_US = 400; //currently not in use
const int32_t DIS_DELTA_STEPS = 10;
const int32_t DIS_MAX_STEPS = 4000;

enum State {
    STOPPED,
    UP,
    DOWN,
    DISCRETE,
    CONTINUOUS
};

int main(void) {
    State curState = State::STOPPED;

    BufferedSerial pc(USBTX, USBRX, BAUD_RATE);
    pc.set_blocking(false);
    
    StepperMotor motor(MOTOR_ENABLE, 
                       MOTOR_STEP, 
                       MOTOR_DIR, 
                       MOTOR_MS1,
                       MOTOR_MS2, 
                       MOTOR_MS3,
                       SLEEP);
    
    CapSense cap_sense(CS_MEASURE, CS_SQUARE);


    DiscreteExperiment discrete_exp(motor, cap_sense, DIS_N_MEASURE, DIS_T_MEASURE_US,
                                    DIS_DELTA_STEPS, DIS_MAX_STEPS);
    
//    ContinuousExperiment continuous_exp(motor, cap_sense);

    while (true) {
        // State behavior
        switch (curState) {
            case State::STOPPED:
                break;
            case State::UP:
                /* move motor up */
                motor.step_positive();
                break;
            case State::DOWN:
                /* move motor down */
                motor.step_negative();
                break;
            case State::DISCRETE:
                /* discrete state behavior */
                discrete_exp.proceed();
                if (discrete_exp.is_done()) {
                    auto& data = discrete_exp.report();
                    printf("Gathered %d data points. Starting data dump...\n", data.size());
                    dump_data(data);
                    curState = State::STOPPED;
                }
                break;
            case State::CONTINUOUS:
                /*continuous state behavior */
//                continuous_exp.proceed();
//                if (continuous_exp.is_done()) {
//                    continuous_exp.report();
//                }
                break;
            default:
                printf("[ERROR, %d] Not a state!\n", __LINE__);
                break;
        }

        // Switch to next state
        char input;
        ssize_t len = pc.read(&input, 1);
        if (len > 0) {
            switch (input) {
                case '1':
                    curState = State::UP;
                    break;
                case '2':
                    curState = State::DOWN;
                    break;
                case 's':
                    printf("Starting DISCRETE experiment\n");
                    curState = State::DISCRETE;
                    discrete_exp.reset();
                    discrete_exp.start();
                    break;
                case '4':
                    curState = State::CONTINUOUS;
//                    continuous_exp.reset();
//                    continuous_exp.start();
                    break;
                case 'e':
                    printf("Stopping\n");
                    if (curState == DISCRETE) { 
                        auto& data = discrete_exp.report();
                        printf("Gathered %d data points. Starting data dump...\n", data.size());
                        dump_data(data);
                        discrete_exp.reset();
                    } else if (curState == CONTINUOUS) {
   //                     continuous_exp.stop();
//                        continuous_exp.report();
                    }
                    curState = State::STOPPED;
                    break;
                default:
                    printf("[ERROR, %d] Not a command!\n", __LINE__);
                    break;
            }
        }
    }
    return 0;
}