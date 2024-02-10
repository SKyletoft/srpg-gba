#include "hexmap.h"
#include "map.h"
#include "scrolling_hexgrid.h"
#include "scrolling_rainbow.h"
#include "state.h"
#include "test_map.h"
#include "tty.h"
#include "util.h"

#include <cstddef>
#include <exception>

extern "C" {
#include <tonc.h>
}

// scrolling_rainbow::ScrollingRainbow scroll_mode{};
// scrolling_hexgrid::ScrollingHexgrid scroll_mode{};
hexmap::Hexmap scroll_mode{test_map::map};
map::MapMode map_mode{};

namespace debug {
extern tty::TtyMode tty_mode;
}

void error_handler() {
	debug::tty_mode.restore();
	debug::tty_mode.clear();
	debug::tty_mode.println("Crashed!  ");

	util::spin();
}

int main() {
	std::set_terminate(error_handler);

	state::next_state = 0;

	size_t mode = state::next_state;
	state::Mode *const modes[] = {
		&scroll_mode,
		&map_mode,
		&debug::tty_mode,
	};

	debug::tty_mode.println("Initialising...");

	modes[mode]->restore();

	for (;;) {
		util::wait_for_drawing_start();
		if (mode != state::next_state) {
			util::wait_for_vsync();
			if (modes[mode]->blackout() || modes[state::next_state]->blackout())
			{
				util::set_screen_to_black();
			}
			modes[mode]->suspend();
			state::last_state = mode;
			mode = state::next_state;
			util::wait_for_vsync();
			modes[mode]->restore();
		}

		key_poll();
		modes[mode]->update();

		for (auto mode : modes) {
			mode->always_update();
		}

		util::wait_for_drawing_complete();
		modes[mode]->vsync_hook();
	}

	util::spin();
	return 0;
}
