#pragma once

#include <mbed.h>

/**
 * Capacitive-sensing module.
 *
 * A capacitive sensor needs to be enabled and disabled periodically.
 * When the capacitive sensor is enabled, the capacitor charges.
 * During this period, try_measure should be called as often as
 * possible. When try_measure returns true, the measurement is ready
 * to be read.
 *
 * At some point, toggle needs to be called again to disable the
 * capacitive sensor. This gives the capacitor time to discharge.
 * Make sure to wait a bit before toggling the sensor back on;
 * otherwise, the capacitor might have leftover charge.
 */
class CapSense {
    public:
    CapSense(PinName measure_pin, PinName square_pin);

    void start();
    void reset();
    bool try_measure();
    uint32_t read_measurement();
    
    private:
    DigitalOut square_wave;
    DigitalIn cap_sense_measure;
    Timer timer;
    uint32_t last_measurement;
    bool measured_this_cycle;
};