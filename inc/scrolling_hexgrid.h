#pragma once

#include "scrolling_map.h"
#include "tiles.h"

extern "C" {
#include <tonc_types.h>
}

namespace scrolling_hexgrid {

using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;

static constexpr int BG0_TILE_SOURCE = 0;
static constexpr int BG0_TILE_MAP = 30;

static constexpr s16 WIDTH = 40 * 3 * 8;
static constexpr s16 HEIGHT = 30 * 4 * 8;

class ScrollingHexgrid : public scrolling_map::ScrollingMap {
  private:
	void load_tilesets() override;
	void load_palettes() override;
	ScreenEntry get_tile(s16, s16) override;

  public:
	ScrollingHexgrid()
		: ScrollingMap(WIDTH, HEIGHT) {}

	void update() override;
	void always_update() override;
	void restore() override;
	void suspend() override;
	void vsync_hook() override;
};

} // namespace scrolling_hexgrid
