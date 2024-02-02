#include "tty.h"

#include "tiles.h"
#include <cstring>
#include <span>

extern "C" {
#include <tonc.h>

extern const u32 sys8Glyphs[192];
}

namespace tty {

using tiles::ScreenEntry;
using tiles::STile;

constexpr size_t BG0_TILE_SOURCE = 0;
constexpr size_t BG0_TILE_MAP = 8;

constexpr u16 get_character_tile_index(char c) {
	if (' ' <= c && c <= '~') {
		return (u16)(c - ' ');
	}
	return 0;
}

void TtyMode::update() {
	if (!(key_held(1 << KI_R) && key_held(1 << KI_L))) {
		state::next_state = 0;
	}
}

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

	this->println("Restored");
}

void TtyMode::vsync_hook() {}

void TtyMode::clear_screen() {
	const ScreenEntry empty{0, 0, 0};
	std::span<u16> tile_map{(u16 *)se_mem[BG0_TILE_MAP], 1024};

	for (u16 &tile : tile_map) {
		tile = empty;
	}
}

void TtyMode::clear_buffer() { this->len = 0; }

void TtyMode::clear() {
	this->clear_buffer();
	this->clear_screen();
}

void TtyMode::println(const char *s) { this->println(s, strlen(s)); }

void TtyMode::println(const char *s, const size_t len) {
	std::span<const char> str{s, len};

	if (len > SIZE) {
		s = s + len + this->len - SIZE;
	} else if (this->len + len > SIZE) {
		this->clear();
	}

	for (char c : str) {
		const u16 index = get_character_tile_index(c);
		se_mem[BG0_TILE_MAP][this->len] = TileRep(index, 0, 0);

		this->len += 1;
	}
}

} // namespace tty
