#pragma once

#include <cstddef>
#include <span>

extern "C" {
#include <tonc.h>
#include <tonc_types.h>
}

namespace sprite {

enum class ColourMode { BPP4 = 0, BPP8 = 1 };

struct alignas(8) Sprite {
	u8 y : 8;
	u8 object_mode : 2;
	u8 graphics_mode : 2;
	bool mosaic : 1;
	ColourMode colour_mode : 1;
	u8 sprite_shape : 2;
	u16 x : 9;
	u8 affine_index : 5;
	u8 size : 2;
	u16 tile_index : 10;
	u8 prio : 2;
	u8 palette : 4;
};
static_assert(sizeof(Sprite) == 4 * sizeof(u16));

static const std::span<volatile Sprite> SPRITE_MEM{(Sprite *)oam_mem, 128};

} // namespace sprite
