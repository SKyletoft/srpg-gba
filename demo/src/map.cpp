#include "map.h"

#include "input.h"
#include "state.h"
#include "tiles.h"
#include "util.h"

#include "config.h"
#include "loading.h"
#include "unit.h"

#include <cstring>

extern "C" {
#include "arrow.h"
}

namespace map {

using input::Button;
using input::InputState;

void Map::update() {
	this->animation_cycle = (u8)((this->animation_cycle + 1) % 1024);

	auto const d =
		config::cursor.move_cursor(config::hexmap.layer0.pos.into<s32>());
	config::hexmap.move_in_bounds(d.x, d.y);
	config::hexmap.update_layer(config::hexmap.layer0);
	config::hexmap.update_layer(config::hexmap.layer1);

	if (input::get_button(Button::R).is_down()
		&& input::get_button(Button::L).is_down())
	{
		state::next_state = 1;
	}

	if (input::get_button(Button::A) == InputState::Pressed) {
		state::next_state = 2;
	}
}

void Map::restore() {
	if (this->blackout()) {
		loading::load_map_graphics();
	}
	if (state::last_state != 2) {
		tiles::SPRITE_PALETTE_MEMORY[0] = *(tiles::Palette *)arrowPal;
	}
	std::memcpy(&tiles::SPRITE_CHARBLOCK[0][1], arrowTiles, sizeof(arrowTiles));
	config::cursor.cursor.animation = {0, 0};
	config::cursor.cursor.render(config::hexmap.layer0.pos);

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

void Map::vsync_hook() {
	config::hexmap.update_camera();
	config::cursor.cursor.render(config::hexmap.layer0.pos);

	u8 animation_cycle = (u8)(this->animation_cycle / 20);

	std::span<unit::Unit> const units{
		config::user_army.data(), config::user_soldier_count
	};
	for (auto &unit : units) {
		unit.render(config::hexmap.layer0.pos, animation_cycle);
	}
}

bool Map::blackout() {
	return !(
		(state::current_state == 0 && state::next_state == 2)
		|| (state::next_state == 0 && state::current_state == 2)
		|| (state::current_state == 0 && state::last_state == 2)
	);
}

} // namespace map
