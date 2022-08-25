#include "data_dumper.h"
#include <algorithm>

void dump_data(std::vector<std::tuple<int32_t, uint32_t>>& data) {
    for (auto& steps_tau_tuple: data) {
        printf("%d,%d\n",
               std::get<0>(steps_tau_tuple),
               std::get<1>(steps_tau_tuple));
    }
};

void clear_data(std::vector<std::tuple<int32_t, uint32_t>>& data) {
    data.clear();
};