#include "mbed.h"
#include "capsense.h"
#include <cstdio>

const PinName CS_MEASURE = D2;
const PinName CS_SQUARE = D3;

// main() runs in its own thread in the OS
int main()
{
    CapSense cap_sense(CS_MEASURE, CS_SQUARE);    

    while (true) {
        cap_sense.start();  // turn it on
        while (!cap_sense.try_measure()) {}
        printf("%d\n", cap_sense.read_measurement());
        // we read a value, so safe to turn off
        cap_sense.reset();
        wait_us(70000);  // wait a bit to discharge

    }
}

