#include "scrolling_rainbow.h"

#include "tiles.h"
#include "util.h"
#include <ranges>
#include <span>

extern "C" {
#include <tonc.h>
#include <tonc_input.h>
#include <tonc_memdef.h>
#include <tonc_memmap.h>
}

#define memcpy_(dest, src) std::memcpy((void *)dest, src, sizeof(src))

namespace rv = std::ranges::views;
using Colour = tiles::Colour;

namespace scrolling_rainbow {

void ScrollingRainbow::load_tilesets(size_t layer) {
	u32 const nums[] = {
		0x11111111,
		0x22222222,
		0x33333333,
		0x44444444,
		0x55555555,
		0x66666666,
		0x77777777,
		0x88888888,
		0x99999999,
		0xAAAAAAAA,
		0xBBBBBBBB,
		0xCCCCCCCC,
		0xDDDDDDDD,
		0xEEEEEEEE,
		0xFFFFFFFF,
	};
	std::span<STile> const tiles{tiles::CHARBLOCKS[layer], 16};

	// Clangd complains but GCC thinks this is fine
	for (auto [tile, num] : rv::zip(tiles, nums)) {
		tile = tiles::STile{{num, num, num, num, num, num, num, num}};
	}
}

void ScrollingRainbow::load_palettes(size_t pal) {
	// memcpy_(pal_bg_mem, grassPal);
	((tiles::Palette *)pal_bg_mem)[pal] = tiles::Palette{
		.colours =
			{
				tiles::TRANSPARENT,
				Colour(31, 0, 0),
				Colour(24, 6, 0),
				Colour(18, 12, 0),
				Colour(12, 18, 0),
				Colour(6, 24, 0),
				Colour(0, 31, 0),
				Colour(0, 24, 6),
				Colour(0, 18, 12),
				Colour(0, 12, 18),
				Colour(0, 6, 24),
				Colour(0, 0, 31),
				Colour(6, 0, 24),
				Colour(12, 0, 18),
				Colour(18, 0, 12),
				Colour(24, 0, 6),
			}
	};
}

ScreenEntry ScrollingRainbow::get_tile(size_t _, s16 x, s16 y) {
	return ScreenEntry((u16)((x * 2 + y * 3) / 2 % 15), 0, 0);
}

} // namespace scrolling_rainbow
