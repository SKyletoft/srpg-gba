#include "hexmap.h"

#include "tiles.h"
#include <cstddef>
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

using tiles::CHARBLOCKS;
using tiles::Palette;
using tiles::PALETTE_MEMORY;

const u16 tile_size = grassTilesLen;

void Hexmap::load_tilesets(Layer &layer) {
	memcpy_((CHARBLOCKS[layer.tile_source] + 0 * tile_size), grassTiles);
	memcpy_((CHARBLOCKS[layer.tile_source] + 1 * tile_size), waterTiles);
	memcpy_((CHARBLOCKS[layer.tile_source] + 2 * tile_size), flowerTiles);
	memcpy_((CHARBLOCKS[layer.tile_source] + 3 * tile_size), water2Tiles);
	memcpy_((CHARBLOCKS[layer.tile_source] + 4 * tile_size), grassTiles);
}

void Hexmap::load_palettes(Layer &) {
	PALETTE_MEMORY[0] = Palette::from_raw(grassPal);
	PALETTE_MEMORY[1] = Palette::from_raw(waterPal);
	PALETTE_MEMORY[2] = Palette::from_raw(flowerPal);
	PALETTE_MEMORY[3] = Palette::from_raw(waterPal);
	PALETTE_MEMORY[4] = Palette::from_raw(grassPal);
}

ScreenEntry Hexmap::get_tile(Layer &layer, s16 x, s16 y) {
	size_t tile_x;
	size_t tile_y;

	if (layer != this->layer0) {
		x -= 2;
		y -= 2;

		tile_x = (size_t)x / 3;
		tile_y = (size_t)(y / 4) * 2 + 1;
	} else {
		tile_x = (size_t)x / 3;
		tile_y = (size_t)(y / 4) * 2;
	}

	u16 index = 0;
	u8 palette = 0;
	if (tile_x < WIDTH - (tile_y & 1) && tile_y < HEIGHT) {
		u8 tile_type = this->map[tile_y, tile_x];
		u16 tile_index = (u16)((y % 4) * 3 + (x % 3) + 1);
		u16 hex_index = tile_type * tile_size;
		palette = tile_type;
		index = tile_index + hex_index;
	}

	return ScreenEntry(index, 0, palette);
}

void SimpleHexmap::load_tilesets(Layer &layer) {
	std::memcpy(
		CHARBLOCKS[layer.tile_source],
		this->tiles.data(),
		this->tiles.size_bytes()
	);
}

void SimpleHexmap::load_palettes(Layer &layer) {
	std::memcpy(
		PALETTE_MEMORY.data(),
		this->palettes.data(),
		this->palettes.size_bytes()
	);
}

} // namespace hexmap
