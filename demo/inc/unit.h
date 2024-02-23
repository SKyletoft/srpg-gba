#pragma once

#include "hexes.h"
#include "sprite.h"

namespace unit {

struct Stats {
	s8 health = 0;

	s8 attack = 0;
	s8 defence = 0;

	s8 magic = 0;
	s8 resistance = 0;

	s8 speed = 0;
	s8 luck = 0;
};

struct Unit {
	sprite::HexSprite sprite{};
	Stats stats{};
	constexpr hexes::CubeCoord &pos() { return this->sprite.pos; }
};

} // namespace unit
