#include "state.h"

#include <cstddef>

namespace state {

size_t next_state = 0;
size_t last_state = 0;

bool Mode::blackout() { return true; }

} // namespace state
