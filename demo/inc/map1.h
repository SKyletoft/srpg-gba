#pragma once

#include "sprite.h"
#include "unit.h"

#include "level.h"

#include <array>

namespace map1 {

using sprite::HexSprite;
using unit::Stats;
using unit::Unit;

// clang-format off
static constexpr std::array<std::array<unsigned char,  10>,  19> map {
	std::array<unsigned char,  10> // What?
	{ 0, 1, 6,18, 1, 1, 1, 1, 1, 1}
	, { 0, 1, 7,19, 1, 1, 1, 2, 1},
	{ 0, 1, 6,18, 1, 1, 2, 1, 1, 0}
	, { 0, 1, 7,19, 1, 1, 1, 1, 1},
	{ 0, 1, 6,18, 1, 1, 1, 1, 1, 1}
	, { 0, 1, 7,21, 1, 1, 1, 1, 0},
	{ 0, 1, 6, 1,15,15,15, 1, 1, 0}
	, { 0, 1, 3, 9, 9,15,15, 2, 0},
	{ 0, 1, 2, 1, 1, 8,15,15, 2, 0}
	, { 0, 1, 1, 1, 1,14,15,14, 0},
	{ 0, 1, 1, 1, 1, 1,14,15,15, 0}
	, { 0, 1, 3, 3, 1, 1, 8,15, 0},
	{ 0, 1, 3, 3, 3, 1,15,15,15, 0}
	, { 0, 1, 3, 3, 1,15, 1, 1, 0},
	{ 0, 1, 2, 1, 1, 3,15, 3, 2, 0}
	, { 0, 1, 2, 1, 1,15, 1, 1, 0},
	{ 0, 1, 1, 1, 3, 1, 1, 1, 4, 0}
	, { 0, 2, 2, 1, 1, 1, 2, 4, 0},
	{ 0, 1, 1, 1, 1, 1, 1, 1, 4, 0}
};
// clang-format on

constexpr Unit enemies[]{
	Unit{
		.name = "Dude #1",
		.portrait = 2,
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(6, 5).to_cube_coord(),
				.centre = {5, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 9,
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
	},
	Unit{
		.name = "Dude #2",
		.portrait = 3,
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(7, 3).to_cube_coord(),
				.centre = {5, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 10,
				.tile_index = 33,
				.prio = 1,
				.palette = 2,
			},
		.stats =
			Stats{
				.health = 16,
				.max_health = 16,
				.attack = 5,
				.defence = 4,
				.magic = 3,
				.resistance = 3,
				.speed = 9,
				.luck = 2,
				.movement = 3,
			},
		.animation_frames = 3,
	},
	Unit{
		.name = "Dude #3",
		.portrait = 3,
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(4, 10).to_cube_coord(),
				.centre = {5, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 12,
				.tile_index = 33,
				.prio = 1,
				.palette = 2,
			},
		.stats =
			Stats{
				.health = 16,
				.max_health = 16,
				.attack = 5,
				.defence = 4,
				.magic = 3,
				.resistance = 3,
				.speed = 9,
				.luck = 2,
				.movement = 3,
			},
		.animation_frames = 3,
	},
	Unit{
		.name = "Dude Leader",
		.portrait = 3,
		.sprite =
			HexSprite{
				.pos = hexes::OffsetXYCoord(5, 15).to_cube_coord(),
				.centre = {5, 0},
				.size = sprite::SpriteSize::x16,
				.hardware_id = 11,
				.tile_index = 33,
				.prio = 1,
				.palette = 2,
			},
		.stats =
			Stats{
				.health = 30,
				.max_health = 30,
				.attack = 5,
				.defence = -2,
				.magic = 3,
				.resistance = 3,
				.speed = 9,
				.luck = 2,
				.movement = 3,
			},
		.animation_frames = 3,
	}
};

DECLARE_LEVEL(Level, map1::map, map1::enemies)

} // namespace map1
