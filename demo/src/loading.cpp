#include "loading.h"

#include "config.h"
#include "tiles.h"

#include <cstring>

extern "C" {
#include <tonc.h>

#include "enemy.h"
#include "font.h"
#include "movement-hl.h"
#include "portraits.h"
}

namespace loading {

void load_ui() {
	tiles::BG_PALETTE_MEMORY[15] = UI_PALETTE;
	tiles::BG_PALETTE_MEMORY[14] = *(tiles::Palette *)portraitsPal;
	std::memcpy(
		tiles::CHARBLOCKS[config::map.ui_layer_source] + 1,
		fontTiles,
		sizeof(fontTiles)
	);
	std::memcpy(
		tiles::CHARBLOCKS[config::map.ui_layer_source] + 98,
		portraitsTiles,
		sizeof(portraitsTiles)
	);
}

void load_sprites() {
	std::memcpy(
		&tiles::SPRITE_CHARBLOCK[0][5 + 4 * 7],
		enemyTiles,
		sizeof(tiles::STile) * 4 * 7
	);
	tiles::SPRITE_PALETTE_MEMORY[1] = BLUE_ACTIVE;
	tiles::SPRITE_PALETTE_MEMORY[2] = RED_ACTIVE;
	tiles::SPRITE_PALETTE_MEMORY[3] = BLUE_USED;
	tiles::SPRITE_PALETTE_MEMORY[4] = RED_USED;
}

void load_tiles() {
	config::hexmap.load_tilesets(config::hexmap.layer0);
	config::hexmap.load_map(config::hexmap.layer0);
	config::hexmap.load_map(config::hexmap.layer1);
	config::hexmap.load_palettes(config::hexmap.layer0);
	tiles::BG_PALETTE_MEMORY[1] = *(tiles::Palette *)movement_hlPal;
	config::hexmap.update_camera();
}

} // namespace loading
