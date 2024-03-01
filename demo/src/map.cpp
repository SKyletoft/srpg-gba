#include "map.h"

#include "state.h"
#include "tiles.h"

#include "config.h"
#include "loading.h"
#include "unit.h"

#include <cstring>
#include <ranges>

extern "C" {
#include "arrow.h"
}

namespace map {

namespace rv = std::ranges::views;

void Map::restore() {
	if (state::last_state == 1 || state::last_state == 4 || state::blacked_out)
	{
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

	config::cursor.cursor.animation.x =
		(s16)((config::cursor.cursor.animation.x * (s16)3) / (s16)4);
	config::cursor.cursor.animation.y =
		(s16)((config::cursor.cursor.animation.y * (s16)3) / (s16)4);

	u8 animation_cycle = (u8)(this->animation_cycle / 20);

	for (auto &unit :
		 std::array{config::user_units(), config::enemy_units()} | rv::join)
	{
		unit.sprite.move();
		unit.render(config::hexmap.layer0.pos, animation_cycle);
	}
}

bool Map::blackout() {
	return state::current_state == 1 || state::last_state == 1;
}

} // namespace map
