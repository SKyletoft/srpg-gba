#include "battle.h"

#include "sprite.h"
#include "tiles.h"

#include <cstring>

extern "C" {
#include <tonc.h>

#include "battle-ani.h"
}

namespace battle {
using tiles::Colour;

void Battle::update() {
	this->time++;
	if (this->time > 5) {
		this->time = 0;
		this->frame = (u8)((this->frame + 1) % 5);
	}
}

void Battle::restore() {
	std::memcpy(
		tiles::SPRITE_CHARBLOCK[0], battle_aniTiles, sizeof(battle_aniTiles)
	);
	tiles::SPRITE_PALETTE_MEMORY[0] = tiles::Palette{{
		tiles::TRANSPARENT,
		Colour(15, 15, 15),
		Colour(15, 0, 0),
		Colour(31, 0, 0),
		Colour(25, 25, 25),
	}};
	tiles::SPRITE_PALETTE_MEMORY[1] = tiles::Palette{{
		tiles::TRANSPARENT,
		Colour(15, 15, 15),
		Colour(0, 0, 15),
		Colour(0, 0, 31),
		Colour(25, 25, 25),
	}};

	// REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

void Battle::vsync_hook() {
	this->left.tile_index = this->frame * 64;
	this->left.write_to_screen(1);

	this->right.tile_index = this->frame * 64;
	this->right.write_to_screen(2);
}

} // namespace battle
