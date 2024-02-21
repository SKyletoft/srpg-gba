#include "map.h"
#include "input.h"
#include "state.h"
#include "tiles.h"
#include "util.h"

#include <cstring>

extern "C" {
#include "arrow.h"
}

namespace map {

using input::Button;
using input::InputState;

void Map::update() {
	auto const d =
		this->cursor.move_cursor(this->hexmap.layer0.pos.into<s32>());
	this->hexmap.move_in_bounds(d.x, d.y);
	this->hexmap.update_layer(this->hexmap.layer0);
	this->hexmap.update_layer(this->hexmap.layer1);

	if (input::get_button(Button::R).is_down()
		&& input::get_button(Button::L).is_down())
	{
		state::next_state = 1;
	}

	if (input::get_button(Button::A) == InputState::Pressed) {
		state::next_state = 2;
	}
}

void Map::always_update() {}

void Map::suspend() {}

void Map::restore() {
	if (this->blackout()) {
		this->hexmap.load_tilesets(this->hexmap.layer0);
		this->hexmap.load_tilesets(this->hexmap.layer1);
		this->hexmap.load_map(this->hexmap.layer0);
		this->hexmap.load_map(this->hexmap.layer1);
		util::wait_for_vsync();
		this->hexmap.load_palettes(this->hexmap.layer0);

		tiles::BG_PALETTE_MEMORY[15] = tiles::Palette{{
			tiles::TRANSPARENT,
			tiles::WHITE,
			// clangd does not consider this a constant expression, gcc does
			tiles::Colour::from_24bit_colour(198, 164, 89),
		}};
		std::memcpy(
			&tiles::SPRITE_PALETTE_MEMORY[0], arrowPal, sizeof(arrowPal)
		);

		this->hexmap.update_camera();
	}
	std::memcpy(&tiles::SPRITE_CHARBLOCK[0][1], arrowTiles, sizeof(arrowTiles));

	REG_BG0CNT = (u16)(BG_CBB((u16)this->hexmap.layer0.tile_source)
					   | BG_SBB((u16)this->hexmap.layer0.tile_map) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(3));
	REG_BG1CNT = (u16)(BG_CBB((u16)this->hexmap.layer1.tile_source)
					   | BG_SBB((u16)this->hexmap.layer1.tile_map) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(3));
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

void Map::vsync_hook() {
	this->hexmap.update_camera();
	this->cursor.cursor.render(this->hexmap.layer0.pos);
}

bool Map::blackout() {
	return !(
		(state::current_state == 0 && state::next_state == 2)
		|| (state::next_state == 0 && state::current_state == 2)
	);
}

} // namespace map
