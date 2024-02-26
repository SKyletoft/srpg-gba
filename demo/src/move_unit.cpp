#include "move_unit.h"

#include "hexes.h"
#include "input.h"
#include "state.h"

#include "config.h"
#include "tiles.h"

namespace move_unit {

using input::Button;
using input::InputState;

void update_palettes_of(
	std::span<hexes::CubeCoord const> highlights, u8 new_palette
) {
	for (auto const &tile : highlights) {
		auto tile_coord =
			(tile.to_pixel_space() - config::hexmap.layer0.pos.into<s32>()) / 8;
		constexpr std::array<size_t, 12> tiles_offsets_in_hex = {
			0, 1, 2, 32, 33, 34, 64, 65, 66, 96, 97, 98
		};

		tiles::ScreenEntry volatile *base;
		if (!tile.is_odd()) {
			base = &tiles::SCREENBLOCKS[config::hexmap.layer0.tile_map]
									   [tile_coord.y * 32 + tile_coord.x];
		} else {
			tile_coord.x += 1;
			base = &tiles::SCREENBLOCKS[config::hexmap.layer1.tile_map]
									   [tile_coord.y * 32 + tile_coord.x];
		}

		for (size_t offset : tiles_offsets_in_hex) {
			tiles::ScreenEntry volatile &se = base[offset];
			tiles::ScreenEntry copy = se;
			copy.palette = new_palette & 0b1111;
			se = copy;
		}
	}
}

void MoveUnit::restore() {
	// this->Map::restore();
	this->highlights =
		config::selected_unit->accessible_tiles(config::hexmap.map);
	update_palettes_of(this->highlights, 1);
}

void MoveUnit::suspend() { update_palettes_of(this->highlights, 0); }

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
		&& config::selected_unit->pos().distance(config::cursor.cursor.pos)
			   <= config::selected_unit->stats.movement)
	{
		state::next_state = 0;
		auto diff = config::selected_unit->pos() - config::cursor.cursor.pos;
		config::selected_unit->pos() = config::cursor.cursor.pos;
		config::selected_unit->sprite.animation =
			diff.to_pixel_space().into<s16>();
		config::selected_unit = nullptr;
	}
}

} // namespace move_unit
