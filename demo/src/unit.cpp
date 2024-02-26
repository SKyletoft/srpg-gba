#include "unit.h"

#include "debug.h"
#include "hexes.h"
#include "sprite.h"
#include "util.h"

#include <algorithm>
#include <queue>
#include <unordered_set>

extern "C" {
#include <tonc_memmap.h>
}

namespace unit {

namespace r = std::ranges;

using hexes::CubeCoord;
using mdspan::Span2d;

void Unit::render(Point<s16> camera_offset, u8 animation_cycle) const {
	sprite::HexSprite sprite{this->sprite};

	sprite.tile_index += (u8)((animation_cycle % this->animation_frames) * 4);

	sprite.render(camera_offset);
}

std::vector<CubeCoord> Unit::accessible_tiles(Span2d<const u8> const &map
) const {
	using CC_Depth = std::pair<CubeCoord, u8>;
	struct CompareDepth {
		bool operator()(const CC_Depth &a, const CC_Depth &b) const {
			return a.second > b.second;
		}
	};
	auto const hex_to_idx = [&](CubeCoord cc) {
		auto const xy = cc.to_offset_xy();
		return (size_t)xy.col * map.width + (size_t)xy.row;
	};

	// Should be π(mov + 0.5)², but close enough
	// size_t const max_len =
	//	(size_t)((this->stats.movement + 1) * (this->stats.movement + 1) * 3);
	static std::vector<CubeCoord> vec{};
	vec.clear();

	static std::priority_queue<CC_Depth, std::vector<CC_Depth>, CompareDepth>
		queue{};
	while (!queue.empty()) {
		queue.pop();
	}

	// std::vector<CC_Depth> queue{};
	// queue.reserve(max_len);
	static std::vector<bool> visited(map.width * map.height, false);
	for (size_t i = 0; i < visited.size(); ++i) {
		visited[i] = false;
	}

	queue.push({this->sprite.pos, 0});
	// queue.push_back({this->sprite.pos, 0});
	visited[hex_to_idx(this->sprite.pos)] = true;

	while (!queue.empty()) {
		// auto [curr, depth] = *queue.begin();
		// queue.erase(queue.begin());
		auto [curr, depth] = queue.top();
		queue.pop();

		if (depth > this->stats.movement) {
			break;
		}

		vec.push_back(curr);

		for (auto const &neighbour : hexes::CUBE_DIRECTION_VECTORS) {
			auto neighbour_ = neighbour + curr;
			auto xy = neighbour_.to_offset_xy();
			if (xy.col < 0 || map.width <= xy.col || xy.row < 0
				|| map.height <= xy.row)
			{
				continue;
			}

			size_t idx = hex_to_idx(neighbour_);
			if (!visited[idx]) {
				// queue.push_back({neighbour_, depth + 1});
				queue.push({neighbour_, depth + 1});
				visited[idx] = true;
			}
		}
	}

	debug::print("Checked tiles: ");
	debug::println((int)visited.size());

	return vec;
}

} // namespace unit
