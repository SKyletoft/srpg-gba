#include "util.h"

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

} // namespace util
