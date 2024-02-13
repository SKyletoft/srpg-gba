#pragma once

#include <cstddef>
#include <span>

extern "C" {
#include <tonc.h>
#include <tonc_types.h>
}

namespace sprite {

enum class ColourMode { BPP4 = 0, BPP8 = 1 };

struct Sprite {
	u8 y : 8 = 0;
	u8 object_mode : 2 = 0;
	u8 graphics_mode : 2 = 0;
	bool mosaic : 1 = 0;
	ColourMode colour_mode : 1 = ColourMode::BPP4;
	u8 sprite_shape : 2 = 0;
	u16 x : 9 = 0;
	u8 affine_index : 5 = 0;
	u8 size : 2 = 0;
	u16 tile_index : 10 = 0;
	u8 prio : 2 = 0;
	u8 palette : 4 = 0;

	u8 _padding = 0;

	void write_to_screen(size_t hardware_sprite_id);
};
static_assert(sizeof(Sprite) == 4 * sizeof(u16));
static_assert(alignof(Sprite) == alignof(u16));

static const std::span<volatile Sprite> SPRITE_MEM{(Sprite *)oam_mem, 128};

} // namespace sprite
