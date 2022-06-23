#include "DigitalIn.h"
#include "DigitalOut.h"
#include "PinNames.h"
#include "PwmOut.h"
#include "ThisThread.h"
#include "mbed.h"
#include "mbed_power_mgmt.h"
#include <chrono>
#include <vector>
#include <memory>

const std::chrono::milliseconds SENSING_PERIOD = 30ms;
const PinName SQUARE_WAVE_PIN = D9;
const PinName MEASURE_PIN = D0;
const int WINDOW_SIZE = 3;

class CapSense {
    public:
    CapSense(PinName measure_pin, PinName square_pin)
        : cap_sense_measure(measure_pin), square_wave(square_pin), measured_this_cycle(false) {}

    bool toggle() {
        bool last_signal = square_wave.read();
        square_wave.write(!last_signal);
        if (!last_signal) {
            measured_this_cycle = false;
            timer.start();
        }
        return !last_signal;
    }

    bool try_measure() {
        if (cap_sense_measure.read() == 1 && !measured_this_cycle) {
            timer.stop();
            last_measurement = timer.elapsed_time().count();
            measured_this_cycle = true;
            timer.reset();
            return true;
        }
        return false;
    }

    long long read_measurement() {
        return last_measurement;
    }

    private:
    DigitalOut square_wave;
    DigitalIn cap_sense_measure;
    Timer timer;
    long long last_measurement;
    bool measured_this_cycle;
};

class RunningAvg {
    public:
    RunningAvg(int window_size) : window_size(window_size), idx(0), window(window_size) {}

    void insert(int value) {
        window.at(idx) = value;
        idx = (idx + 1) % window_size;
    }

    int average() {
        int sum = 0;
        for (int val : window) {
            sum += val;
        }
        return sum/window_size;
    }

    private:
    const int window_size;
    std::vector<int> window;
    int idx;
};

int main()
{   
    printf("hano! starting up\n");
    ThisThread::sleep_for(1000ms);

    CapSense cs(MEASURE_PIN, SQUARE_WAVE_PIN);

    Ticker t;
    auto ticker_func = [&cs]() { cs.toggle(); };
    t.attach(ticker_func, SENSING_PERIOD);

    RunningAvg ra(WINDOW_SIZE);
    printf("hano! finished setting up\n");

    int print_count = 0;
    while (true) {
        if (cs.try_measure()) {
            long long val = cs.read_measurement();
            ra.insert(val);

            if (print_count == 10) {
                printf("%d\n", ra.average());
                print_count = 0;
            }
            print_count += 1;
        }
    }
}
