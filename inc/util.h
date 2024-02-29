#pragma once

#include <exception>
#include <span>

extern "C" {
#include <tonc_types.h>
}

#define assert(x)                                                              \
	if (!(x))                                                                  \
		std::terminate();

namespace util {

extern vu32 x;
[[noreturn]] void spin();

void set_screen_to_black();
void clear_layer(size_t);

template <typename T> bool contains(std::span<const T> container, T const &elem) {
	return container.begin() <= &elem && &elem < container.end();
}

void wait_for_drawing_start();
void wait_for_drawing_complete();
void wait_for_vsync();

} // namespace util
