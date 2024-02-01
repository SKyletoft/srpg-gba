#include "tty.h"

extern "C" {
#include <tonc.h>
}

namespace tty {

void TtyMode::update() {}

void TtyMode::always_update() {}

void TtyMode::suspend() {}

void TtyMode::restore() {
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
	tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));
}

void TtyMode::vsync_hook() {}

} // namespace tty
