/// This file is intended to be included in the game's source files,
/// not the engine's

#include "hexmap.h"
#include "popup.h"
#include "test_map.h"
#include "tty.h"

#include "soundbank.h"

#include <array>
#include <span>

#define EXPORT(list, song)                                                     \
	std::span<state::Mode *const> modes{list};                                 \
	u32 the_startup_song = (u32)song;

namespace debug {
extern tty::TtyMode tty_mode;
}

namespace config {

hexmap::Hexmap hexmap{test_map::map};
popup::PopupMenu popup{};

std::array<state::Mode *, 4> const modes_data{
	&hexmap,
	&debug::tty_mode,
	&debug::tty_mode,
	&popup,
};

u32 startup_song = MOD_BAD_APPLE;

EXPORT(modes_data, startup_song);

} // namespace config
