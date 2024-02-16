#pragma once

#include <tuple>

extern "C" {
#include <tonc_memmap.h>
#include <tonc_types.h>
}

namespace perf {

void record_frame();
std::tuple<u32, u32> get_perf_data();

} // namespace perf
