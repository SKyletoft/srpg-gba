#include "debug.h"
#include "export.h"
#include "tiles.h"
#include "tty.h"
#include <array>

#include "context_menu.h"
#include "map.h"
#include "soundbank.h"
#include "test_map.h"

namespace debug {
extern tty::TtyMode tty_mode;
}

namespace config {

using tiles::BG_PALETTE_MEMORY;
using tiles::SPRITE_PALETTE_MEMORY;

map::Map map{test_map::map};
context_menu::ContextMenu popup{
	{"Red",
	 []() {
		 debug::println("Setting text to red");
		 BG_PALETTE_MEMORY[15].colours[1] = tiles::RED;
		 SPRITE_PALETTE_MEMORY[0].colours[1] = tiles::RED;
	 }},
	{"Green",
	 []() {
		 debug::println("Setting text to green");
		 BG_PALETTE_MEMORY[15].colours[1] = tiles::GREEN;
		 SPRITE_PALETTE_MEMORY[0].colours[1] = tiles::GREEN;
	 }},
	{"Blue",
	 []() {
		 debug::println("Setting text to blue");
		 BG_PALETTE_MEMORY[15].colours[1] = tiles::BLUE;
		 SPRITE_PALETTE_MEMORY[0].colours[1] = tiles::BLUE;
	 }},
	{"Exit", []() { state::next_state = 0; }},
};

std::array<state::Mode *, 3> const modes_data{
	&map,
	&debug::tty_mode,
	&popup,
};

u32 startup_song = MOD_BAD_APPLE;

EXPORT(modes_data, startup_song);

} // namespace config
