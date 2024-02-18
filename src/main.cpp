#include "input.h"
#include "perf.h"
#include "state.h"
#include "tty.h"
#include "util.h"
#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include <span>


extern "C" {
	#include <tonc_bios.h>
	#include <tonc_memdef.h>
	#include <tonc_irq.h>
	#include <tonc.h>
}

namespace config {
extern std::span<state::Mode *const> modes;
}

namespace debug {
extern tty::TtyMode tty_mode;
}

extern "C" [[noreturn]] void _exit(int) {
	debug::tty_mode.restore();
	debug::tty_mode.clear();
	debug::tty_mode.println("Crashed!");

	util::spin();
}

int main() {

	irq_init(nullptr);
	irq_add(II_VBLANK, mmVBlank);
	irq_enable(II_VBLANK);

	mmInitDefault((mm_addr) soundbank_bin, 8);
	mmStart(MOD_BAD_APPLE, MM_PLAY_LOOP);
	state::next_state = 0;

	state::current_state = state::next_state;
	debug::tty_mode.println("Initialising...");

	config::modes[state::current_state]->restore();

	for (;;) {

		mmFrame();
		//util::wait_for_vsync();
		VBlankIntrWait();
		util::wait_for_drawing_start();
		if (state::current_state != state::next_state) {

			VBlankIntrWait();
			//util::wait_for_vsync();
			if (config::modes[state::current_state]->blackout()
				|| config::modes[state::next_state]->blackout())
			{
				util::set_screen_to_black();
			}
			config::modes[state::current_state]->suspend();
			state::last_state = state::current_state;
			state::current_state = state::next_state;
			VBlankIntrWait();
			//util::wait_for_vsync();
			config::modes[state::current_state]->restore();
		}

		input::poll();
		config::modes[state::current_state]->update();
		for (auto mode : config::modes) {
			mode->always_update();
		}

		perf::record_frame();
		mmFrame();
		util::wait_for_drawing_complete();
		config::modes[state::current_state]->vsync_hook();

	}

	util::spin();
	return 0;
}
