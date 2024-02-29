#include "unit.h"

#include "browse.h"
#include "config.h"
#include "debug.h"
#include "hexes.h"
#include "perf.h"
#include "sprite.h"
#include "util.h"

#include "set.h"
#include <algorithm>
#include <queue>
#include <ranges>

extern "C" {
#include <tonc_memmap.h>
}

namespace unit {

namespace r = std::ranges;
namespace rv = std::ranges::views;

using hexes::CubeCoord;
using mdspan::Span2d;

u8 cost(u8 terrain) {
	switch (terrain) {
	case 3:
	case 6:
	case 7:
	case 8:
	case 9:
	case 14:
		return 100;
	case 18:
	case 19:
	case 15:
	case 21:
		return 1;
	default:
		return 3;
	}
}

void Unit::render(Point<s16> camera_offset, u8 animation_cycle) const {
	sprite::HexSprite sprite{this->sprite};

	sprite.tile_index += (u8)((animation_cycle % this->animation_frames) * 4);

	sprite.render(camera_offset);
}

Set<CubeCoord> Unit::accessible_tiles(Span2d<const u8> const &map) const {
	util::wait_for_drawing_start();
	u16 start = REG_VCOUNT;
	size_t start_frame_id = perf::get_frame();

	const s16 height = (s16)map.height;
	const s16 width = (s16)map.width;

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
	size_t const max_len =
		(size_t)((this->stats.movement + 1) * (this->stats.movement + 1) * 3);
	Set<CubeCoord> out{};
	// out.reserve(max_len);

	std::priority_queue<CC_Depth, std::vector<CC_Depth>, CompareDepth> queue{};
	// std::queue<CC_Depth> queue{};

	std::vector<bool> visited(map.width * map.height, false);
	for (Unit const &unit :
		 std::array{config::user_units(), config::enemy_units()} | rv::join)
	{
		visited[hex_to_idx(unit.pos())] = true;
	}

	queue.push({this->sprite.pos, 0});
	visited[hex_to_idx(this->sprite.pos)] = true;

	u16 at_loop = REG_VCOUNT;
	size_t at_loop_frame_id = perf::get_frame();

	int loops = 0;
	while (!queue.empty()) {
		loops++;
		auto [curr, depth] = queue.top();
		// auto [curr, depth] = queue.front();
		queue.pop();

		out.insert(curr);
		browse::update_palette_of_tile(curr, 1);

		for (auto const &neighbour : hexes::CUBE_DIRECTION_VECTORS) {
			auto neighbour_ = neighbour + curr;
			auto xy = neighbour_.to_offset_xy();
			if (xy.col < 0 || width <= xy.col || xy.row < 0 || height <= xy.row)
			{
				continue;
			}

			u8 depth_ = depth + cost(map[(size_t)xy.row, (size_t)xy.col]);
			size_t idx = hex_to_idx(neighbour_);
			if (!visited[idx] && depth_ <= this->stats.movement) {
				queue.push({neighbour_, depth_});
				visited[idx] = true;
			}
		}
	}

	u16 end = REG_VCOUNT;
	size_t end_frame_id = perf::get_frame();

	debug::print("Checked tiles: ");
	debug::println((int)visited.size());
	debug::println("");
	debug::println(loops);
	debug::println("");
	debug::println(start);
	debug::println((int)start_frame_id);
	debug::println("");
	debug::println(at_loop);
	debug::println((int)at_loop_frame_id);
	debug::println("");
	debug::println(end);
	debug::println((int)end_frame_id);

	return out;
}

bool Unit::is_user() const {
	return config::user_army.begin() <= this && this < config::user_army.end();
}

} // namespace unit
