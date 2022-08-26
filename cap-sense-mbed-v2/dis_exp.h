#pragma once

#include "stepper.h"
#include "capsense.h"
#include <cstdint>
#include <vector>
#include <tuple>

class DiscreteExperiment {
    public:
    DiscreteExperiment(StepperMotor& motor, CapSense& capsense, 
                       int32_t n_measure, uint32_t t_measure_us, 
                       int32_t delta_steps, int32_t max_steps);
    void proceed();
    bool is_done();
    /**
     * Returns a vector of tuples of (steps, tau in microseconds)
     */
    std::vector<std::tuple<int32_t, uint32_t>>& report(); /*steps instead of dis */
    void start();
    void reset();

    private:
    enum State {
        IDLE,
        MOVING,
        MEASURING,
        DONE
    };

    private:
    StepperMotor& motor;
    CapSense& capsense;

    int32_t n_measure;
    uint32_t t_measure_us;
    int32_t delta_steps;
    int32_t max_steps;

    std::vector<std::tuple<int32_t, uint32_t>> data;
    State state;
    
    int32_t move_counter;
    int32_t measu_counter;
    int32_t steps_taken;

}; 