#pragma once

#include <cstddef>

#include "hexes.h"
#include "scrolling_map.h"

extern "C" {
#include <tonc_types.h>
}

namespace cursor_scroller {

using hexes::CubeCoord;

class CursorScroller : public scrolling_map::ScrollingMap {
  protected:
	CubeCoord cursor{};

	void update() override;

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
};

} // namespace cursor_scroller
