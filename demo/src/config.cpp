#include "config.h"

#include "debug.h"
#include "export.h"
#include "hexes.h"
#include "sprite.h"
#include "tiles.h"
#include "tty.h"
#include <array>

#include "context_menu.h"
#include "map.h"
#include "soundbank.h"
#include "test_map.h"
#include "unit.h"

namespace debug {
extern tty::TtyMode tty_mode;
}

namespace config {

std::array<Unit, 12> user_army{
	Unit{
		.sprite =
			sprite::HexSprite{
				.pos = hexes::OffsetXYCoord(4, 4).to_cube_coord(),
				.centre = {4, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 1,
				.tile_index = 5,
				.palette = 1,
			},
		.stats =
			Stats{
				.health = 20,
				.attack = 10,
				.defence = 8,
				.magic = 3,
				.resistance = 3,
				.speed = 9,
				.luck = 2,
			},
		.animation_frames = 3,
	},
};
size_t user_soldier_count = 1;

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
