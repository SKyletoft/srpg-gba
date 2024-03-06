#pragma once

#include "level.h"
#include "sprite.h"
#include "unit.h"

#include <array>

namespace map2 {

using sprite::HexSprite;
using unit::Stats;
using unit::Unit;

// clang-format off
static constexpr std::array<std::array<unsigned char,  20>,  9> map {
	std::array<unsigned char,  20> // What?
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	, { 0, 1, 1, 1, 1,15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1},
	{ 0, 1, 1, 1,15,15, 1, 1, 1, 1, 1,15,15,15,15, 1, 1, 1, 2, 1}
	, { 0, 1, 1,15, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 2, 1},
	{ 0,15, 1,15, 2, 1, 1, 1, 1, 3,15,15, 1, 1, 1,15,15, 1, 1, 1}
	, { 0,15,15, 1, 1, 1, 1,15,15,15, 2, 1,15, 1, 3, 2,15, 1, 1},
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1,15, 1, 2, 3,15, 1, 1}
	, { 0, 1, 3, 1, 1, 1, 1, 1, 2, 2, 1,15,15, 3, 1,15,15, 1, 1},
	{ 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
// clang-format on

constexpr Unit type_1(s16 x, s16 y, std::string_view name, u8 num) {
	return Unit{
		.name = name,
		.portrait = 2,
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(x, y).to_cube_coord(),
				.centre = {5, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = num,
				.tile_index = 33,
				.prio = 1,
				.palette = 2,
			},
		.stats =
			Stats{
				.health = 18,
				.max_health = 18,
				.attack = 10,
				.defence = 2,
				.magic = 3,
				.resistance = 3,
				.speed = 9,
				.luck = 2,
				.movement = 3,
			},
		.animation_frames = 3,
	};
}

constexpr Unit type_2(s16 x, s16 y, std::string_view name, u8 num) {
	return Unit{
		.name = name,
		.portrait = 2,
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(x, y).to_cube_coord(),
				.centre = {5, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = num,
				.tile_index = 33,
				.prio = 1,
				.palette = 2,
			},
		.stats =
			Stats{
				.health = 10,
				.max_health = 18,
				.attack = 15,
				.defence = 4,
				.magic = 3,
				.resistance = 3,
				.speed = 9,
				.luck = 2,
				.movement = 3,
			},
		.animation_frames = 3,
	};
}

constexpr Unit type_3(s16 x, s16 y, std::string_view name, u8 num) {
	return Unit{
		.name = name,
		.portrait = 2,
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(x, y).to_cube_coord(),
				.centre = {5, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = num,
				.tile_index = 33,
				.prio = 1,
				.palette = 2,
			},
		.stats =
			Stats{
				.health = 40,
				.max_health = 18,
				.attack = 10,
				.defence = -2,
				.magic = 3,
				.resistance = 3,
				.speed = 9,
				.luck = 2,
				.movement = 0,
			},
		.animation_frames = 3,
	};
}

constexpr Unit enemies[]{
	type_1(7, 5, "Dude #1", 9),
	type_1(5, 1, "Dude #2", 10),
	type_1(9, 5, "Dude #3", 11),
	type_1(11, 4, "Dude #4", 12),
	type_1(12, 5, "Dude #5", 13),
	type_1(11, 7, "Dude #6", 14),
	type_1(11, 2, "Dude #7", 15),
	type_1(14, 2, "Dude #8", 16),
	type_1(15, 4, "Dude #9", 17),
	type_1(15, 7, "Dude #10", 18),
	type_3(17, 6, "Dude Leader", 19),
};

DECLARE_LEVEL(Level, map2::map, map2::enemies)

} // namespace map2
