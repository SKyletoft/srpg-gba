#include "move_unit.h"

#include "hexes.h"
#include "input.h"
#include "state.h"

#include "config.h"
#include "tiles.h"
#include "unit.h"

#include "set.h"
#include <ranges>

namespace move_unit {

namespace r = std::ranges;
namespace rv = std::ranges::views;

using input::Button;
using input::InputState;
using unit::Unit;

void update_palette_of_tile(CubeCoord const tile, u8 new_palette) {
	auto px = tile.to_pixel_space() - config::hexmap.layer0.pos.into<s32>();
	if (px.x < -24 || 240 < px.x || px.y < -24 || 160 < px.y) {
		return;
	}

	size_t end = 12;
	size_t start = 0;
	// Only draw 2/3 columns for the partially visible tile
	if (px.x == 240) {
		end = 8;
	}
	if (px.x < -16) {
		start = 4;
	}

	auto tile_coord = tile.to_pixel_space() / 8;
	// Stored by column so we can skip the last column when it goes off-screen
	constexpr std::array<size_t, 12> tiles_offsets_in_hex = {
		0, 32, 64, 96, 1, 33, 65, 97, 2, 34, 66, 98
	};

	tiles::ScreenEntry volatile *base;
	if (!tile.is_odd()) {
		base = &tiles::SCREENBLOCKS[config::hexmap.layer0.tile_map]
								   [tile_coord.y * 32];
	} else {
		tile_coord.x += 1;
		base = &tiles::SCREENBLOCKS[config::hexmap.layer1.tile_map]
								   [tile_coord.y * 32];
	}

	for (size_t i = start; i < end; ++i) {
		size_t idx = tiles_offsets_in_hex[i] + (size_t)tile_coord.x;
		// This only runs *once*, so not a (% 32) or else only
		// the top line of the hex gets highlighted
		if (idx >= 32) {
			idx -= 32;
		}
		tiles::ScreenEntry volatile &se = base[idx];
		tiles::ScreenEntry copy = se;
		copy.palette = new_palette & 0b1111;
		se = copy;
	}
}

void update_palettes_of(Set<CubeCoord> const &highlights, u8 new_palette) {
	for (auto const &tile : highlights) {
		update_palette_of_tile(tile, new_palette);
	}
}

void MoveUnit::restore() {
	this->Map::restore();
	config::highlights =
		config::selected_unit->accessible_tiles(config::hexmap.map);
	update_palettes_of(config::highlights, 1);
}

void MoveUnit::suspend() {
	update_palettes_of(config::highlights, 0);
	config::highlights.clear();
}

void MoveUnit::update() {
	this->Map::update();

	// Terminal on both shoulders
	if (input::get_button(Button::R).is_down()
		&& input::get_button(Button::L).is_down())
	{
		state::next_state = 1;
		config::selected_unit = nullptr;
	}

	if (input::get_button(Button::B) == InputState::Pressed) {
		state::next_state = 0;
		config::selected_unit = nullptr;
	}

	if (input::get_button(Button::A) == InputState::Pressed
		&& config::highlights.contains(config::cursor.pos()))
	{
		auto const targetted_enemy =
			r::find_if(config::enemy_units(), [&](Unit const &unit) {
				return unit.pos() == config::cursor.pos();
			});
		if (targetted_enemy != config::enemy_units().end()) {
			state::next_state = 5;
			config::battle_ani.set_combatants(*config::selected_unit, *targetted_enemy);
		} else {
			state::next_state = 0;
			auto diff =
				config::selected_unit->pos() - config::cursor.cursor.pos;
			config::selected_unit->pos() = config::cursor.cursor.pos;
			config::selected_unit->sprite.animation =
				diff.to_pixel_space().into<s16>();
			config::selected_unit = nullptr;
		}
	}
}

} // namespace move_unit
