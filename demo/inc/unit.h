#pragma once

#include "hexes.h"
#include "mdspan.h"
#include "point.h"
#include "sprite.h"

#include "set.h"

namespace unit {

using point::Point;

struct Stats {
	s8 health = 0;

	s8 attack = 0;
	s8 defence = 0;

	s8 magic = 0;
	s8 resistance = 0;

	s8 speed = 0;
	s8 luck = 0;

	s8 movement = 5;
};

struct Unit {
	sprite::HexSprite sprite{};
	Stats stats{};

	u8 animation_frames = 1;

	constexpr hexes::CubeCoord const &pos() const { return this->sprite.pos; }
	constexpr hexes::CubeCoord &pos() { return this->sprite.pos; }
	void render(Point<s16>, u8) const;

	Set<hexes::CubeCoord>
	accessible_tiles(mdspan::Span2d<const u8> const &) const;
};

} // namespace unit
