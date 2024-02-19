#pragma once

#include <cstddef>
#include <span>

extern "C" {
#include <tonc.h>
#include <tonc_types.h>
}

namespace sprite {

enum class ColourMode { BPP4 = 0, BPP8 = 1 };

struct alignas(8) __attribute((packed)) Sprite {
	// attr0
	u8 y : 8 = 0;
	u8 object_mode : 2 = 0;
	u8 graphics_mode : 2 = 0;
	bool mosaic : 1 = 0;
	ColourMode colour_mode : 1 = ColourMode::BPP4;
	u8 shape : 2 = 0;

	// attr1
	u16 x : 9 = 0;
	u8 _pad0 : 3 = 0;
	bool horizontal_flip : 1 = false;
	bool vertical_flip : 1 = false;
	u8 size : 2 = 0;

	// attr2
	u16 tile_index : 10 = 0;
	u8 prio : 2 = 0;
	u8 palette : 4 = 0;

	// padding
	u16 _pad1 = 0;

	void write_to_screen(size_t hardware_sprite_id);
};
static_assert(sizeof(Sprite) == sizeof(OBJ_ATTR));
static_assert(alignof(Sprite) >= alignof(OBJ_ATTR));

static constexpr Sprite X8{
	.shape = 0b00,
	.size = 0b00,
};
static constexpr Sprite X16{
	.shape = 0b00,
	.size = 0b01,
};
static constexpr Sprite X32{
	.shape = 0b00,
	.size = 0b10,
};
static constexpr Sprite X64{
	.shape = 0b00,
	.size = 0b11,
};

// Due to fun literal language bugs, this is read only and you have to use the
// `Sprite::write_to_screen` function to write
//
// https://timsong-cpp.github.io/lwg-issues/3813
static const std::span<const Sprite> SPRITE_MEM{(Sprite *)MEM_OAM, 128};

} // namespace sprite
