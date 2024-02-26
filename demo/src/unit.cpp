#include "unit.h"
#include "hexes.h"
#include "sprite.h"

namespace unit {

using hexes::CubeCoord;
using mdspan::Span2d;

void Unit::render(Point<s16> camera_offset, u8 animation_cycle) const {
	sprite::HexSprite sprite{this->sprite};

	sprite.tile_index += (u8)((animation_cycle % this->animation_frames) * 4);

	sprite.render(camera_offset);
}

std::vector<CubeCoord> Unit::accessible_tiles(Span2d<u8> const &map) const {
	std::vector vec{this->sprite.pos};
	return vec;
}

} // namespace unit
