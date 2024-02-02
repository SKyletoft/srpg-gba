#pragma once

extern "C" {
#include <tonc_types.h>
}

namespace util {

extern vu32 x;
void spin();

void set_screen_to_black();

void wait_for_drawing_start();
void wait_for_drawing_complete();
void wait_for_vsync();

} // namespace util
