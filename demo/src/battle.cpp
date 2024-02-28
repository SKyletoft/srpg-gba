#include "battle.h"

#include "sprite.h"
#include "tiles.h"

#include <cstring>

extern "C" {
#include <tonc.h>

#include "battle-ani.h"
}

namespace battle {

void Battle::update() {}

void Battle::restore() {
	std::memcpy(
		&tiles::SPRITE_CHARBLOCK[0][1], battle_aniTiles, sizeof(battle_aniTiles)
	);
	// tiles::SPRITE_PALETTE_MEMORY[0] = tiles::Palette{{
	//	tiles::TRANSPARENT,
	//	tiles::BLACK,
	//	tiles::Colour(31, 14, 14),
	//	tiles::Colour(31, 25, 25),
	// }};
	tiles::SPRITE_PALETTE_MEMORY[0] = *(tiles::Palette *)battle_aniPal;

	// REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;

	auto sprite = sprite::HardwareSprite{
		.object_mode = sprite::ObjectMode::Normal,
		.tile_index = 1,
		.palette = 0,
	};
	sprite.set_size(sprite::SpriteSize::x64);
	sprite.write_to_screen(1);
}

void Battle::vsync_hook() {}

} // namespace battle
