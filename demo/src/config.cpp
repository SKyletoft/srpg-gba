#include "config.h"

#include "browse.h"
#include "cursor_scroller.h"
#include "debug.h"
#include "export.h"
#include "hexes.h"
#include "sprite.h"
#include "state.h"
#include "tiles.h"
#include "tty.h"

#include "context_menu.h"
#include "loading.h"
#include "map.h"
#include "soundbank.h"
#include "test_map.h"
#include "unit.h"

#include "set.h"
#include <array>

namespace debug {
extern tty::TtyMode tty_mode;
}

namespace config {
using sprite::HexSprite;

cursor_scroller::CursorScroller cursor{};
hl_map::HighlightMap hexmap{test_map::map};

Unit *selected_unit = nullptr;
Set<hexes::CubeCoord> highlights{};
std::vector<Unit *> neighbouring_enemies{};
std::array<Unit, 8> user_army{
	Unit{
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(4, 4).to_cube_coord(),
				.centre = {4, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 1,
				.tile_index = 33,
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
				.movement = 8,
			},
		.animation_frames = 3,
	},
	Unit{
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(4, 5).to_cube_coord(),
				.centre = {4, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 2,
				.horizontal_flip = true,
				.tile_index = 33,
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
				.movement = 3,
			},
		.animation_frames = 3,
	},
};
size_t user_soldier_count = 2;

std::array<Unit, 20> enemy_army{
	Unit{
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(6, 5).to_cube_coord(),
				.centre = {4, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 9,
				.tile_index = 33,
				.palette = 2,
			},
		.stats = Stats{},
		.animation_frames = 3,
	},
	Unit{
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(7, 3).to_cube_coord(),
				.centre = {4, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 3,
				.tile_index = 33,
				.palette = 2,
			},
		.stats = Stats{},
		.animation_frames = 3,
	}
};
size_t enemy_soldier_count = 2;

std::span<Unit> user_units() {
	return std::span<Unit>{user_army.data(), user_soldier_count};
}
std::span<Unit> enemy_units() {
	return std::span<Unit>{enemy_army.data(), enemy_soldier_count};
}

battle::Battle battle_ani{};
browse::DefaultMap map{};
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
context_menu::ContextMenu movement_popup{
	{"Attack",
	 []() {
		 map.state = browse::MapState::Animating;
		 state::next_state = 0;
		 auto diff = config::selected_unit->pos() - config::cursor.cursor.pos;
		 config::selected_unit->pos() = config::cursor.cursor.pos;
		 config::selected_unit->sprite.animation =
			 diff.to_pixel_space().into<s16>();
		 browse::update_palettes_of(config::highlights, 0);

		 highlights.clear();
		 for (Unit *enemy : neighbouring_enemies) {
			 highlights.insert(enemy->pos());
		 }
		 browse::update_palettes_of(highlights, 1);
	 }},
	{"Wait",
	 []() {
		 map.state = browse::MapState::Animating;
		 neighbouring_enemies.clear();
		 config::selected_unit->sprite.move_to(config::cursor.pos());
		 browse::update_palettes_of(config::highlights, 0);
		 config::highlights.clear();
		 state::next_state = 0;
	 }},
};

std::array<state::Mode *, 7> const modes_data{
	&map,
	&debug::tty_mode,
	&popup,
	nullptr,
	&battle_ani,
	&movement_popup,
	nullptr,
};

u32 startup_song = MOD_BAD_APPLE;

EXPORT(modes_data, startup_song);

} // namespace config
