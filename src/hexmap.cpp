#include "hexmap.h"

#include "tiles.h"
#include <cstdlib>
#include <cstring>

extern "C" {
#include "flower.h"
#include "grass.h"
#include "water.h"
#include "water2.h"
}

#define memcpy_(dest, src) std::memcpy((void *)dest, src, sizeof(src))

namespace hexmap {

const u16 tile_size = grassTilesLen;

void Hexmap::load_tilesets(Layer &layer) {
	memcpy_((tiles::CHARBLOCKS[layer.tile_source] + 0 * tile_size), grassTiles);
	memcpy_((tiles::CHARBLOCKS[layer.tile_source] + 1 * tile_size), waterTiles);
	memcpy_(
		(tiles::CHARBLOCKS[layer.tile_source] + 2 * tile_size), flowerTiles
	);
	memcpy_(
		(tiles::CHARBLOCKS[layer.tile_source] + 3 * tile_size), water2Tiles
	);
	memcpy_((tiles::CHARBLOCKS[layer.tile_source] + 4 * tile_size), grassTiles);
}

void Hexmap::load_palettes(Layer &) {
	tiles::PALETTE_MEMORY[0] = tiles::Palette::from_raw(grassPal);
	tiles::PALETTE_MEMORY[1] = tiles::Palette::from_raw(waterPal);
	tiles::PALETTE_MEMORY[2] = tiles::Palette::from_raw(flowerPal);
	tiles::PALETTE_MEMORY[3] = tiles::Palette::from_raw(waterPal);
	tiles::PALETTE_MEMORY[4] = tiles::Palette::from_raw(grassPal);
}

ScreenEntry Hexmap::get_tile(Layer const &layer, s16 x, s16 y) {
	if (layer != this->layer0) {
		x += 1;
		y += 2;
	}

	std::srand((unsigned)(((y / 4) << 16) | (x / 3)));
	u8 rng = (u8)(rand() % 4);

	u16 tile_index = (u16)((y % 4) * 3 + (x % 3) + 1);
	u16 hex_index = rng * tile_size;

	return ScreenEntry(tile_index + hex_index, 0, rng);
}

} // namespace hexmap
