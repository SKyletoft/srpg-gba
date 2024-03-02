#include "loading.h"

#include "config.h"
#include "tiles.h"
#include "util.h"

#include <cstring>
#include <ranges>

extern "C" {
#include <tonc.h>

#include "arrow.h"
#include "enemy.h"
#include "lyn.h"
#include "movement-hl.h"
}

namespace loading {

namespace rv = std::ranges::views;

void load_map_graphics() {
	for (auto i : rv::iota(0uz, 128uz)) {
		sprite::HardwareSprite::hide(i);
	}

	config::hexmap.load_tilesets(config::hexmap.layer0);
	config::hexmap.load_tilesets(config::hexmap.layer1);
	config::hexmap.load_map(config::hexmap.layer0);
	config::hexmap.load_map(config::hexmap.layer1);
	util::wait_for_vsync();
	config::hexmap.load_palettes(config::hexmap.layer0);

	tiles::BG_PALETTE_MEMORY[1] = *(tiles::Palette *)movement_hlPal;
	tiles::BG_PALETTE_MEMORY[15] = UI_PALETTE;

	config::hexmap.update_camera();

	std::memcpy(
		&tiles::SPRITE_CHARBLOCK[0][5], lynTiles, sizeof(tiles::STile) * 4 * 7
	);
	std::memcpy(
		&tiles::SPRITE_CHARBLOCK[0][5 + 4 * 7],
		enemyTiles,
		sizeof(tiles::STile) * 4 * 7
	);
	tiles::SPRITE_PALETTE_MEMORY[1] = BLUE_ACTIVE;
	tiles::SPRITE_PALETTE_MEMORY[2] = RED_ACTIVE;
	tiles::SPRITE_PALETTE_MEMORY[3] = BLUE_USED;
	tiles::SPRITE_PALETTE_MEMORY[4] = RED_USED;

	REG_BG0CNT = (u16)(BG_CBB(config::hexmap.layer0.tile_source)
					   | BG_SBB(config::hexmap.layer0.tile_map) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(3));
	REG_BG1CNT = (u16)(BG_CBB(config::hexmap.layer1.tile_source)
					   | BG_SBB(config::hexmap.layer1.tile_map) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(3));
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

} // namespace loading
