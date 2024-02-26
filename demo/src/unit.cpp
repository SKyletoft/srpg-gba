#include "unit.h"

#include "hexes.h"
#include "sprite.h"

#include <algorithm>

namespace unit {

namespace r = std::ranges;

using hexes::CubeCoord;
using mdspan::Span2d;

void Unit::render(Point<s16> camera_offset, u8 animation_cycle) const {
	sprite::HexSprite sprite{this->sprite};

	sprite.tile_index += (u8)((animation_cycle % this->animation_frames) * 4);

	sprite.render(camera_offset);
}

std::vector<CubeCoord> Unit::accessible_tiles(Span2d<u8> const &map) const {
	std::vector<CubeCoord> vec{};
	// Todo: Replace this with a min-heap
	std::vector<std::pair<CubeCoord, u8>> queue{{this->sprite.pos, 0}};

	while (!queue.empty()) {
		auto const [curr, depth] = queue.back();
		queue.pop_back();
		vec.push_back(curr);

		if (depth >= this->stats.movement) {
			continue;
		}
		for (auto neighbour : hexes::CUBE_DIRECTION_VECTORS) {
			neighbour += curr;
			if (r::find_if(queue, [&](auto c) { return c.first == neighbour; })
				== queue.end())
			{
				queue.push_back({neighbour, depth + 1});
			}
		}
	}

	return vec;
}

} // namespace unit
