#pragma once

extern "C" {
#include <tonc_types.h>
}

namespace sprite {

enum class ColourMode { BPP4 = 0, BPP8 = 1 };

struct alignas(2) __attribute((packed)) Attribute0 {
	u8 y : 8;
	u8 object_mode : 2;
	u8 graphics_mode : 2;
	bool mosaic : 1;
	ColourMode colour_mode : 1;
	u8 sprite_shape : 2;
};
static_assert(sizeof(Attribute0) == sizeof(u16));

struct alignas(2) __attribute((packed)) Attribute1 {
	u16 x : 9;
	u8 affine_index : 5;
	u8 size : 2;
};
static_assert(sizeof(Attribute1) == sizeof(u16));

struct alignas(2) __attribute((packed)) Attribute2 {
	u16 tile_index : 10;
	u8 prio : 2;
	u8 palette : 4;
};
static_assert(sizeof(Attribute2) == sizeof(u16));

struct alignas(8) Sprite {
	Attribute0 attr0;
	Attribute1 attr1;
	Attribute2 attr2;
};
static_assert(sizeof(Sprite) == 4 * sizeof(u16));

} // namespace sprite
