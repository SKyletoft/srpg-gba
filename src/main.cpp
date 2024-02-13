#include "input.h"
#include "state.h"
#include "tty.h"
#include "util.h"

#include <exception>
#include <span>

namespace config {
extern std::span<state::Mode *const> modes;
}

namespace debug {
extern tty::TtyMode tty_mode;
}

void error_handler() {
	debug::tty_mode.restore();
	debug::tty_mode.clear();
	debug::tty_mode.println("Crashed!");

	util::spin();
}

int main() {
	std::set_terminate(error_handler);

	state::next_state = 0;

	state::current_state = state::next_state;
	debug::tty_mode.println("Initialising...");

	config::modes[state::current_state]->restore();

	for (;;) {
		util::wait_for_drawing_start();
		if (state::current_state != state::next_state) {
			util::wait_for_vsync();
			if (config::modes[state::current_state]->blackout()
				|| config::modes[state::next_state]->blackout())
			{
				util::set_screen_to_black();
			}
			config::modes[state::current_state]->suspend();
			state::last_state = state::current_state;
			state::current_state = state::next_state;
			util::wait_for_vsync();
			config::modes[state::current_state]->restore();
		}

		input::poll();
		config::modes[state::current_state]->update();

		for (auto mode : config::modes) {
			mode->always_update();
		}

		util::wait_for_drawing_complete();
		config::modes[state::current_state]->vsync_hook();
	}

	util::spin();
	return 0;
}
