#include "config.h"

#include "cursor_scroller.h"
#include "debug.h"
#include "dialogue.h"
#include "export.h"
#include "hexes.h"
#include "image.h"
#include "level.h"
#include "main_menu.h"
#include "map.h"
#include "mdspan.h"
#include "overlay.h"
#include "sprite.h"
#include "state.h"
#include "stats.h"
#include "tiles.h"
#include "tty.h"

#include "context_menu.h"
#include "map1.h"
#include "map2.h"
#include "soundbank.h"
#include "test_map.h"

#include "set.h"
#include <array>

namespace debug {
extern tty::TtyMode tty_mode;
}

namespace config {
using hexes::CubeCoord;

cursor_scroller::CursorScroller cursor{};
hl_map::HighlightMap hexmap{test_map::map};

Unit *selected_unit = nullptr;
CubeCoord original_pos{};
Set<CubeCoord> highlights{};
std::vector<Unit *> neighbouring_enemies{};
Set<Unit *> used{};
std::array<Unit, 8> const default_user_army{
	Unit{
		.name = "Guy #1",
		.portrait = 0,
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(4, 4).to_cube_coord(),
				.centre = {4, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 1,
				.horizontal_flip = true,
				.tile_index = 33,
				.prio = 1,
				.palette = 1,
			},
		.stats =
			Stats{
				.health = 20,
				.max_health = 20,
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
		.name = "Guy #2",
		.portrait = 1,
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(4, 5).to_cube_coord(),
				.centre = {4, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 2,
				.horizontal_flip = true,
				.tile_index = 33,
				.prio = 1,
				.palette = 1,
			},
		.stats =
			Stats{
				.health = 20,
				.max_health = 20,
				.attack = 8,
				.defence = 9,
				.magic = 3,
				.resistance = 3,
				.speed = 9,
				.luck = 2,
				.movement = 3,
			},
		.animation_frames = 3,
	},
};

std::array<Unit, 8> user_army = {};
size_t user_soldier_count = 2;

std::array<Unit, 20> enemy_army{};
size_t enemy_soldier_count = 0;

std::span<Unit> user_units() {
	return std::span<Unit>{user_army.data(), user_soldier_count};
}
std::span<Unit> enemy_units() {
	return std::span<Unit>{enemy_army.data(), enemy_soldier_count};
}

overlay::Overlay overlay{};
battle::Battle battle_ani{};
map::Map map{};
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
	{"Heal all",
	 []() {
		 for (auto &unit : user_units()) {
			 unit.stats.health = unit.stats.max_health;
		 }
	 }},
	{"End turn",
	 []() {
		 for (auto &unit : config::user_units()) {
			 config::used.insert(&unit);
		 }
		 state::next_state = 0;
	 }},
	{"Exit",
	 []() {
		 state::next_state = 3;
		 image.bg = image::Background::TitleScreen;
	 }},
};
context_menu::ContextMenu movement_popup{
	{"Attack",
	 []() {
		 map.state = map::MapState::Animating;
		 state::next_state = 0;
		 config::original_pos = config::selected_unit->pos();
		 config::selected_unit->sprite.move_to(config::cursor.pos());
		 map::update_palettes_of(config::highlights, 2);

		 highlights.clear();
		 for (Unit *enemy : neighbouring_enemies) {
			 highlights.insert(enemy->pos());
		 }
		 map::update_palettes_of(highlights, 1);
	 }},
	{"Wait",
	 []() {
		 map.state = map::MapState::Animating;
		 neighbouring_enemies.clear();
		 config::selected_unit->sprite.move_to(config::cursor.pos());
		 map::update_palettes_of(config::highlights, 2);
		 config::highlights.clear();
		 state::next_state = 0;
	 }},
};

stats::Stats stats{};
dialogue::Dialogue dialogue{dialogue::TEXT};

image::Image image{};

main_menu::MainMenu game_over{
	{"Return to title",
	 []() {
		 state::next_state = 3;
		 image.bg = image::Background::TitleScreen;
	 }},
};

main_menu::MainMenu main_menu{
	{"Start",
	 []() {
		 state::next_state = 11;
		 dialogue.restart();
		 auto level = map1::Level{};
		 level::load_level(level);
		 map::cycle_hovered_unit();
	 }},
	{"Level 1",
	 []() {
		 auto level = map1::Level{};
		 level::load_level(level);
		 config::map.end_enemy_turn();
		 state::next_state = 6;
		 overlay.image = overlay::Image::Rout;
	 }},
	{"Level 2",
	 []() {
		 auto level = map2::Level{};
		 level::load_level(level);
		 config::map.end_enemy_turn();
		 state::next_state = 6;
		 overlay.image = overlay::Image::Rout;
	 }},
};

main_menu::MainMenu win{
	{"Continue",
	 []() {
		 auto level = map2::Level{};
		 level::load_level(level);
		 config::map.end_enemy_turn();
		 map::cycle_hovered_unit();
		 state::next_state = 6;
		 overlay.image = overlay::Image::Rout;
	 }},
	{"Return to title",
	 []() {
		 state::next_state = 3;
		 image.bg = image::Background::TitleScreen;
	 }},
};

std::array<state::Mode *, 12> const modes_data{
	&map,
	&debug::tty_mode,
	&popup,
	&image,
	&battle_ani,
	&movement_popup,
	&overlay,
	&game_over,
	&main_menu,
	&win,
	&stats,
	&dialogue,
};

u32 startup_song = MOD_BAD_APPLE;
u32 player_song = MOD_BAD_APPLE;
u32 enemy_song = MOD_MEGALOVANIA;
u32 menu_song = MOD_ZELDA_OPENING;
u32 jingle_battle = MOD_BATTLE;
u32 sfx_swoosh = SFX_SWOOSH;
u32 sfx_fwoop = SFX_FWOOP;

EXPORT(modes_data, startup_song);

} // namespace config
