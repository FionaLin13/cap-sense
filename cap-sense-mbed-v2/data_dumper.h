#pragma once

#include <mbed.h>
#include <vector>
#include <tuple>

/**
* Takes data collected from experiment (steps, tau) and sends it over serial.
*/
void dump_data(std::vector<std::tuple<int32_t, uint32_t>>& data);

void clear_data(std::vector<std::tuple<int32_t, uint32_t>>& data);
