/// This file is intended to be included in the game's source files,
/// not the engine's

#include "export.h"
#include "hexmap.h"
#include "popup.h"
#include "tty.h"

#include "map.h"
#include "soundbank.h"
#include "test_map.h"

#include <array>

namespace debug {
extern tty::TtyMode tty_mode;
}

namespace config {

// hexmap::Hexmap hexmap{test_map::map};
map::Map map{test_map::map};
popup::PopupMenu popup{};

std::array<state::Mode *, 4> const modes_data{
	&map,
	&debug::tty_mode,
	&debug::tty_mode,
	&popup,
};

u32 startup_song = MOD_BAD_APPLE;

EXPORT(modes_data, startup_song);

} // namespace config
