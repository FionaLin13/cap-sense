#include <cstdint>
#include <cstdio>
#include <mbed.h>
#include "dis_exp.h"
#include "stepper.h"

DiscreteExperiment::DiscreteExperiment(StepperMotor& motor, CapSense& capsense, 
                                       int32_t n_measure, uint32_t t_measure_us, 
                                       int32_t delta_steps, int32_t max_steps)
    : motor(motor), capsense(capsense), n_measure(n_measure),
      t_measure_us(t_measure_us), delta_steps(delta_steps), 
      max_steps(max_steps), steps_taken(0) {}
    
void DiscreteExperiment::proceed() {
    const int DISCHARGE_TOLERANCE = 5;
    switch (state) {
        case State::IDLE:
            break;
        case State::DONE:
            break;
        case State::MOVING:
            if (move_counter != 0 && steps_taken < max_steps) {
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
                measu_counter = n_measure;
            }
            break;
        case State::MEASURING:
            if (measu_counter != 0) {
                if (measu_counter < 0) {
                    printf("[bug] Oh shit....\n");
                }
                // using channel 11 on adafruit mpr121
                uint32_t tau = steps_taken;

                data.push_back({steps_taken, tau});
                measu_counter--;

                wait_us(DISCHARGE_TOLERANCE*tau);
            } else if (steps_taken < max_steps) {
                state = MOVING;
                move_counter = delta_steps;
            } else {
                state = DONE;
                steps_taken = 0; // clears the steps_taken counter so that next measure can run
            }
            break;
    }
}


bool DiscreteExperiment::is_done() {
    return state == DONE;
}

std::vector<std::tuple<int32_t, uint32_t>>& DiscreteExperiment::report() {
    return data;
}

void DiscreteExperiment::start() {
    state = MOVING;
    move_counter = delta_steps;
}

void DiscreteExperiment::reset() {
    state = IDLE;
}
