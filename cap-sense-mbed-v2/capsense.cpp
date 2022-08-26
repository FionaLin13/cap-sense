#include <cstdint>
#include <cstdio>
#include <mbed.h>
#include "capsense.h"

CapSense::CapSense(PinName measure_pin, PinName square_pin)
    : cap_sense_measure(measure_pin), square_wave(square_pin), 
      measured_this_cycle(false) {}

void CapSense::start() {
    square_wave.write(1);
    timer.start();

    measured_this_cycle = false;
}

void CapSense::reset() {
    square_wave.write(0);
    wait_us(10000);
}

bool CapSense::try_measure() {
    if (cap_sense_measure.read() == 1 && !measured_this_cycle)
         {
            
            timer.stop();
            last_measurement = timer.elapsed_time().count();
            measured_this_cycle = true;
            timer.reset();
            return true;

        }
        return false;
}

uint32_t CapSense::read_measurement() {
    return last_measurement;
}


    