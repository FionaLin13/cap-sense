#include <cmath>
#include <cstdint>
#include <mbed.h>
#include "stepper.h"
#include "capsense.h"

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

const int32_t DIS_N_MEASURE = 50;
const uint32_t DIS_T_MEASURE_US = 400; //currently not in use
const int32_t DIS_DELTA_STEPS = 25;
const int32_t DIS_MAX_STEPS = 800;

int32_t move_counter = 0;
int32_t measu_counter = 0;
int32_t steps_taken = 0;

enum State {
    STOPPED,
    UP,
    DOWN,
    DISCRETE,
    MEASURING
};

State state;

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
                if (move_counter != 0 && steps_taken < DIS_MAX_STEPS) {
                if (move_counter < 0) {
                    printf("[bug] Oh shit....\n");
                }
                motor.step_positive();
                wait_us(100);
                printf("motor stepping now\n");
                printf("move_counter %d\n", move_counter);
                printf("steps taken %d\n", steps_taken);
                move_counter--;
                steps_taken++;
            } else {
                printf("state went to measuring \n");
                state = MEASURING;
                measu_counter = DIS_N_MEASURE;
            }
            break;
            case State::MEASURING:
                if (measu_counter != 0) {
                    wait_us(100);
                    measu_counter--;
                }
                else if (steps_taken >= DIS_MAX_STEPS) {
                    state = STOPPED;
                }
                else {
                state = DISCRETE;
                move_counter = DIS_DELTA_STEPS;
            }
            break;
            default:
                printf("[ERROR, %d] Not a state!\n", __LINE__);
                break;
            }
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
                case '3':
                    printf("Starting DISCRETE experiment\n");
                    curState = State::DISCRETE;
                    break;
                case '0':
                    printf("Stopping\n");
                    if (curState == DISCRETE) { 
                        printf("Stopping measurement.");
                    }
                    curState = State::STOPPED;
                    break;
                default:
                    printf("[ERROR, %d] Not a command!\n", __LINE__);
                    break;
            }
        }
    return 0;
    }