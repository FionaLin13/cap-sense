#include <mbed.h>

const PinName MOTOR_ENABLE = D15;
const PinName MOTOR_STEP = D9;
const PinName MOTOR_DIR = D13;
const PinName MOTOR_MS1 = D12;
const PinName MOTOR_MS2 = D11;
const PinName MOTOR_MS3 = D10;
const PinName SLEEP = D1;



int main() {
    DigitalOut step(MOTOR_STEP);

    while (true) {
        step.write(1);
        wait_us(50000);
        step.write(0);
        wait_us(40000);
    }
}