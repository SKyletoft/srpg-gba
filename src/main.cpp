#include "map.h"
#include "state.h"
#include "tty.h"
#include "util.h"

#include <cstddef>
#include <exception>

extern "C" {
#include <tonc.h>
}

map::MapMode map_mode{};
tty::TtyMode tty_mode{};

void error_handler() {
	tty_mode.restore();
	tty_mode.clear();
	tty_mode.println("Crashed!  ");

	util::spin();
}

int main() {
	std::set_terminate(error_handler);

	state::next_state = 0;

	size_t mode = state::next_state;
	state::Mode *const modes[2] = {
		&map_mode,
		&tty_mode,
	};

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
