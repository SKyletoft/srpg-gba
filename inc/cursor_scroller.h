#pragma once

#include <cstddef>

#include "hexes.h"
#include "scrolling_map.h"
#include "sprite.h"

extern "C" {
#include <tonc_types.h>
}

namespace cursor_scroller {

using hexes::CubeCoord;

static constexpr s16 COOLDOWN = 8;

class CursorScroller : public scrolling_map::ScrollingMap {
  protected:
	CubeCoord cursor = CubeCoord::from_offset_xy({.col = 5, .row = 5});
	sprite::Sprite cursor_sprite {
		// 16x16
		.shape = 0,
		.size = 1,
		// First tile is reserved for transparency
		.tile_index = 1,
	};
	/// Up, Down, Left, Right
	std::array<s16, 4> directional_cooldowns{0, 0, 0, 0};

	void handle_input();

  public:
	CursorScroller(s16 width, s16 height)
		: ScrollingMap(width, height) {}

	CursorScroller(
		s16 width, s16 height, size_t bg0_tile_source, size_t bg0_tile_map,
		size_t bg1_tile_source, size_t bg1_tile_map
	)
		: ScrollingMap(
			width, height, bg0_tile_source, bg0_tile_map, bg1_tile_source,
			bg1_tile_map
		) {}

	void update() override;
	void vsync_hook() override;
	void restore() override;
};

} // namespace cursor_scroller
