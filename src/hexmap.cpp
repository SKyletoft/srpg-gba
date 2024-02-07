#include "hexmap.h"

#include "tiles.h"
#include <cstring>

extern "C" {
#include "grass.h"
}

#define memcpy_(dest, src) std::memcpy((void *)dest, src, sizeof(src))

namespace hexmap {

void Hexmap::load_tilesets(size_t layer) {
	memcpy_(tiles::CHARBLOCKS[layer], grassTiles);
}

void Hexmap::load_palettes(size_t) {
	memcpy_(tiles::PALETTE_MEMORY.data(), grassPal);
}

ScreenEntry Hexmap::get_tile(size_t layer, s16 x, s16 y) {
	if (layer != this->bg0_tile_map) {
		x += 1;
		y += 2;
	}

	auto const hex00 = ScreenEntry(1, 0, 0);
	auto const hex01 = ScreenEntry(2, 0, 0);
	auto const hex02 = ScreenEntry(3, 0, 0);
	auto const hex10 = ScreenEntry(4, 0, 0);
	auto const hex11 = ScreenEntry(5, 0, 0);
	auto const hex12 = ScreenEntry(6, 0, 0);
	auto const hex20 = ScreenEntry(7, 0, 0);
	auto const hex21 = ScreenEntry(8, 0, 0);
	auto const hex22 = ScreenEntry(9, 0, 0);
	auto const hex30 = ScreenEntry(10, 0, 0);
	auto const hex31 = ScreenEntry(11, 0, 0);
	auto const hex32 = ScreenEntry(12, 0, 0);

	ScreenEntry const hexes[4][3] = {
		{hex00, hex01, hex02},
		{hex10, hex11, hex12},
		{hex20, hex21, hex22},
		{hex30, hex31, hex32},
	};

	return hexes[y % 4][x % 3];
}

} // namespace hexmap
