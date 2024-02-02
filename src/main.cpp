#include "map.h"
#include "state.h"
#include "tty.h"
#include "util.h"

#include <cstddef>

extern "C" {
#include <tonc.h>
}

map::MapMode map_mode{};
tty::TtyMode tty_mode{};

int main() {
	state::next_state = 0;

	size_t mode = state::next_state;
	state::Mode *modes[2] = {
		&map_mode,
		&tty_mode,
	};

	modes[mode]->restore();

	for (;;) {
		if (mode != state::next_state) {
			modes[mode]->suspend();
			mode = state::next_state;
			modes[mode]->restore();
		}

		key_poll();
		modes[mode]->update();

		for (auto mode : modes) {
			mode->always_update();
		}

		vid_vsync();
		modes[mode]->vsync_hook();
	}

	util::spin();
	return 0;
}
