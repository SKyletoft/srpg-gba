#include "map.h"
#include "state.h"
#include "util.h"

#include <cstddef>

extern "C" {
#include <tonc.h>
}

map::MapMode map_mode{};

int main() {
	size_t mode = 0;
	state::Mode *modes[1] = {
		&map_mode,
	};

	modes[mode]->restore();

	for (;;) {
		key_poll();
		modes[mode]->update();
		vid_vsync();
		modes[mode]->vsync_hook();
	}

	util::spin();
	return 0;
}
