#pragma once

#include "sprite.h"

namespace unit {

struct Stats {
	s8 health;

	s8 attack;
	s8 defence;

	s8 magic;
	s8 resistance;

	s8 speed;
	s8 luck;
};

struct Unit {
	sprite::HexSprite sprite{};
	Stats stats;
};

} // namespace unit
