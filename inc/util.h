#pragma once

extern "C" {
#include <tonc_types.h>
}

#include <exception>
#define assert(x)                                                              \
	if (!(x))                                                                  \
		std::terminate();

namespace util {

extern vu32 x;
void spin();

void set_screen_to_black();
void clear_layer(size_t);

void wait_for_drawing_start();
void wait_for_drawing_complete();
void wait_for_vsync();

} // namespace util
