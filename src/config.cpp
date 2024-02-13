/// This file is intended to be included in the game's source files,
/// not the engine's

#include "hexmap.h"
#include "map.h"
#include "popup.h"
#include "scrolling_hexgrid.h"
#include "scrolling_rainbow.h"
#include "test_map.h"
#include "tty.h"

#include <array>
#include <span>

namespace debug {
extern tty::TtyMode tty_mode;
}

namespace config {

scrolling_rainbow::ScrollingRainbow rainbow{};
scrolling_hexgrid::ScrollingHexgrid hex{};
hexmap::Hexmap hexmap{test_map::map};
map::MapMode old_map{};
popup::PopupMenu popup{};

std::array<state::Mode *, 4> const modes_data{
	&hexmap,
	&old_map,
	&debug::tty_mode,
	&popup,
};
std::span<state::Mode *const> modes{modes_data};

} // namespace config
