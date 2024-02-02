#include "tty.h"

#include "tiles.h"
#include <cstring>
#include <span>

extern "C" {
#include "fe7.h"
#include <tonc.h>
#include <tonc_memmap.h>
#include <tonc_tte.h>

extern const u32 sys8Glyphs[192];
}

namespace tty {

using tiles::STile;
using tiles::TileRep;

constexpr size_t BG0_TILE_SOURCE = 0;
constexpr size_t BG0_TILE_MAP = 8;

constexpr u16 get_character_tile_index(char c) {
	if (' ' <= c && c <= '~') {
		return (u16)(c - ' ');
	}
	return 0;
}

void TtyMode::update() {}

void TtyMode::always_update() {}

void TtyMode::suspend() {}

constexpr tiles::Palette YELLOW_ON_BLACK = tiles::Palette{{
	tiles::BLACK,
	tiles::YELLOW,
}};

/// len is counting compressed bytes
void decompress_1bpp_to_4bpp(
	void *dest, const u32 *__restrict src, size_t len
) {
	STile *dest_tile = (STile *)dest;
	STile *end = (STile *)dest + len;

	for (size_t i = 0; dest_tile != end; i += 2) {
		u32 bottom = src[i];
		u32 top = src[i + 1];

		STile tile{};
		for (size_t j = 0; j < 32; j += 2) {
			u32 low_nibble_mask = 1 << j;
			u32 high_nibble_mask = 1 << (j + 1);

			tile.raw[j / 2] = ((bottom & low_nibble_mask) != 0)
							  | (byte)(((bottom & high_nibble_mask) != 0) << 4);

			tile.raw[j / 2 + 16] =
				((top & low_nibble_mask) != 0)
				| (byte)(((top & high_nibble_mask) != 0) << 4);
		}

		*dest_tile = tile;
		dest_tile++;
	}
}

void TtyMode::restore() {
	decompress_1bpp_to_4bpp(tile_mem[BG0_TILE_SOURCE], sys8Glyphs, '~' - ' ');
	std::memcpy(&pal_bg_mem[0], &YELLOW_ON_BLACK, sizeof(YELLOW_ON_BLACK));

	this->clear();

	REG_BG0CNT =
		BG_CBB(BG0_TILE_SOURCE) | BG_SBB(BG0_TILE_MAP) | BG_4BPP | BG_REG_32x32;
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
}

void TtyMode::vsync_hook() {}

void TtyMode::clear() {
	this->len = 0;

	const TileRep empty{0, 0, 0};
	std::span<u16> tile_map{(u16 *)se_mem[BG0_TILE_MAP], 1024};

	for (u16 &tile : tile_map) {
		tile = empty;
	}
}

} // namespace tty
