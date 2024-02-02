#pragma once

#include <span>
#include <tonc_memmap.h>

extern "C" {
#include <tonc_types.h>
}

namespace tiles {

class Colour {
  private:
	u16 data;

	static constexpr u16 convert(u8 red, u8 green, u8 blue) {
		constexpr u16 MASK = 0b11111;
		const u16 be =
			(u16)(((blue & MASK) << 10) | ((green & MASK) << 5) | (red & MASK));

		return be;
	};

	struct RGB {
		u8 red;
		u8 green;
		u8 blue;
	};

  public:
	constexpr Colour(u8 red, u8 green, u8 blue)
		: data(convert(red, green, blue)) {}

	constexpr Colour(RGB rgb)
		: Colour(rgb.red, rgb.green, rgb.blue) {}

	constexpr Colour(u16 raw)
		: data(raw) {}

	constexpr Colour()
		: data(0) {}
};
static_assert(sizeof(Colour) == sizeof(u16));

static constexpr Colour RED = Colour({
	.red = 0b11100,
	.green = 0,
	.blue = 0,
});
static constexpr Colour GREEN = Colour({
	.red = 0,
	.green = 0b11100,
	.blue = 0,
});
static constexpr Colour YELLOW = Colour({
	.red = 0b01110,
	.green = 0b01110,
	.blue = 0,
});
static constexpr Colour BLUE = Colour({
	.red = 0,
	.green = 0,
	.blue = 0b11100,
});
static constexpr Colour BLACK = Colour({
	.red = 0,
	.green = 0,
	.blue = 0,
});
static constexpr Colour WHITE = Colour({
	.red = 0b11111,
	.green = 0b11111,
	.blue = 0b11111,
});
static constexpr Colour TRANSPARENT = BLACK;

struct Palette {
	Colour colours[16];
};

static_assert(sizeof(Palette[16]) == 0x0200);

struct __attribute((packed)) ScreenEntry {
	u8 tile : 8;
	u8 palette : 8;
const std::span<Palette, 16> PALETTE_MEMORY{(Palette *)pal_bg_mem, 16};

};

/// 4 bpp tile
union STile {
	byte raw[32];
	u16 index_quads[16];
	u32 index_octs[8];
};
static_assert(sizeof(STile) == 32);

union Charblock {
	byte raw[16 * 1024];
	STile stiles[512];
};

static_assert(sizeof(Charblock) == 16 * 1024);
const std::span<Charblock> CHARBLOCKS{(Charblock *)tile_mem, 6};

union TileRep {
	u16 raw;
	struct __attribute((packed)) bitflags {
		u16 index : 10;
		u8 flips : 2;
		u8 palette : 4;
	} bitflags;

	constexpr TileRep(u16 index, u8 flips, u8 palette)
		: bitflags({
			.index = (u16)(index & 0b1111111111),
			.flips = (u8)(flips & 0b11),
			.palette = (u8)(palette & 0b1111),
		}) {}

	constexpr operator u16() const { return this->raw; }
	constexpr operator u16() { return this->raw; }
};
static_assert(sizeof(TileRep) == sizeof(u16));

} // namespace tiles
