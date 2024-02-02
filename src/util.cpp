#include "util.h"
#include "tiles.h"

extern "C" {
#include <tonc_memmap.h>
}

namespace util {

// An infinite loop without a sideeffect (like setting a global
// variable) is UB and may be optimised away by the compiler.
// Returning from main might actually be fine and handled by the
// runtime, but let's not rely on that until we've checked it out
// with gdb
vu32 x;
void spin() {
	for (u32 i = 0;; ++i) {
		x = i;
	}
}

void set_screen_to_black() {
	for (auto &palette : tiles::PALETTE_MEMORY) {
		palette = tiles::BLACK_ON_BLACK;
	}
	REG_DISPCNT = 0;
}

} // namespace util
