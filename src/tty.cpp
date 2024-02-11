#include "tty.h"

#include "tiles.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <span>
#include <stdarg.h>

extern "C" {
#include <tonc.h>
#include <tonc_input.h>

extern const u32 sys8Glyphs[192];
}

namespace tty {

using tiles::CHARBLOCKS;
using tiles::Palette;
using tiles::SCREENBLOCKS;
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

constexpr size_t get_grid_index(size_t i) {
	size_t d = i / TtyMode::LINE_LEN;
	size_t r = i % TtyMode::LINE_LEN;

	return d * 32 + r;
}

void TtyMode::update() {
	if (!(key_held(1 << KI_R) && key_held(1 << KI_L))) {
		state::next_state = 0;
	}

	if (key_held(1 << KI_LEFT)) {
		this->print("<");
	}
	if (key_held(1 << KI_RIGHT)) {
		this->print(">");
	}
	if (key_held(1 << KI_UP)) {
		this->print("^");
	}
	if (key_held(1 << KI_DOWN)) {
		this->print("v");
	}
	if (key_held(1 << KI_A)) {
		this->print("a");
	}
	if (key_held(1 << KI_B)) {
		this->print("b");
	}
	if (key_held(1 << KI_START)) {
		this->println("");
	}
	if (key_held(1 << KI_SELECT)) {
		this->clear();
	}
}

void TtyMode::always_update() {}

void TtyMode::suspend() { this->in_focus = false; }

constexpr Palette YELLOW_ON_BLACK = Palette{{
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
	this->in_focus = true;

	decompress_1bpp_to_4bpp(CHARBLOCKS[BG0_TILE_SOURCE], sys8Glyphs, '~' - ' ');

	this->clear_screen();

	vid_vsync();
	tiles::PALETTE_MEMORY[0] = YELLOW_ON_BLACK;
	REG_BG0CNT =
		BG_CBB(BG0_TILE_SOURCE) | BG_SBB(BG0_TILE_MAP) | BG_4BPP | BG_REG_32x32;
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

	this->draw_buffer();
}

void TtyMode::vsync_hook() {}

void TtyMode::clear_screen() {
	if (!this->in_focus) {
		return;
	}

	const ScreenEntry empty{0, 0, 0};
	std::span<u16> tile_map{(u16 *)SCREENBLOCKS[BG0_TILE_MAP], 1024};

	for (u16 &tile : tile_map) {
		tile = empty;
	}
}

void TtyMode::clear_buffer() { this->len = 0; }

void TtyMode::clear() {
	this->clear_buffer();
	this->clear_screen();
}

void TtyMode::scroll_down() {
	std::memmove(this->buffer.data(), this->buffer.data() + SIZE / 2, SIZE / 2);
	this->len = SIZE / 2;
	this->clear_screen();
	this->draw_buffer();
}

void TtyMode::draw_char(size_t i) {
	if (!this->in_focus) {
		return;
	}
	size_t const idx = get_grid_index(i);
	u16 const c = get_character_tile_index(this->buffer[i]);
	SCREENBLOCKS[BG0_TILE_MAP][idx] = ScreenEntry(c, 0, 0);
}

void TtyMode::draw_buffer() {
	if (!this->in_focus) {
		return;
	}
	for (size_t i = 0; i < this->len; ++i) {
		this->draw_char(i);
	}
}

void TtyMode::pad_to_newline() {
	size_t until = (this->len / TtyMode::LINE_LEN + 1) * TtyMode::LINE_LEN;
	size_t len;
	for (len = this->len; len < until; ++len) {
		this->buffer[len] = ' ';
	}
	this->len = len;
}

void TtyMode::println(const char *s) {
	this->print(s);
	this->print("\n");
}

void TtyMode::println(const char *s, const size_t len) {
	this->print(s);
	this->print("\n");
}

void TtyMode::printf(const char *format, ...) {
	char buf[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	this->print(buf);
}

void TtyMode::print(const char *s) { this->print(s, strlen(s)); }

// len should *not* include the null terminator
void TtyMode::print(const char *s, const size_t len) {
	std::span<const char> str{s, len};

	for (char c : str) {
		if (this->len >= SIZE) {
			this->scroll_down();
		}
		if (c == 0) {
			break;
		}
		if (c == '\n') {
			this->pad_to_newline();
			continue;
		}
		this->buffer[this->len] = c;

		this->draw_char(this->len - 1);

		this->len += 1;
	}
}

} // namespace tty
