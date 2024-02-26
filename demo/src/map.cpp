#include "map.h"

#include "state.h"
#include "tiles.h"

#include "config.h"
#include "loading.h"
#include "unit.h"

#include <cstring>

extern "C" {
#include "arrow.h"
}

namespace map {

using unit::Unit;

void Map::update() {}

void Map::restore() {
	config::hexmap.load_map(config::hexmap.layer0);
	config::hexmap.load_map(config::hexmap.layer1);

	if (state::blacked_out) {
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
		unit.sprite.move();
		unit.render(config::hexmap.layer0.pos, animation_cycle);
	}
}

bool Map::blackout() {
	return state::current_state == 1 || state::last_state == 1;
}

} // namespace map
