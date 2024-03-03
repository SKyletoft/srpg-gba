#pragma once

#include "hexes.h"
#include "mdspan.h"
#include "point.h"
#include "sprite.h"

#include "set.h"
#include <string_view>
#include <vector>

namespace unit {

using hexes::CubeCoord;
using mdspan::Span2d;
using point::Point;

struct Stats {
	s8 health = 0;
	s8 max_health = 0;

	s8 attack = 0;
	s8 defence = 0;

	s8 magic = 0;
	s8 resistance = 0;

	s8 speed = 0;
	s8 luck = 0;

	s8 movement = 5;
};

struct Unit {
	std::string_view name = "Untitled solider";
	u8 portrait = 0;

	sprite::HexSprite sprite{};
	Stats stats{};

	u8 animation_frames = 1;

	constexpr CubeCoord const &pos() const { return this->sprite.pos; }
	constexpr CubeCoord &pos() { return this->sprite.pos; }
	void render(Point<s16>, u8) const;

	Set<CubeCoord>
	accessible_tiles(mdspan::Span2d<const u8> const &, bool = false) const;
	std::vector<std::pair<Unit *, CubeCoord>>
	attackable_units(Set<CubeCoord> const &) const;

	bool is_user() const;
};

} // namespace unit
