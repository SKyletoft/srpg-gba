#pragma once

#include "scrolling_map.h"
#include "tiles.h"

extern "C" {
#include <tonc_types.h>
}

namespace scrolling_rainbow {

using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;

static constexpr int BG0_TILE_SOURCE = 0;
static constexpr int BG0_TILE_MAP = 30;

static constexpr s16 WIDTH = (s16)32000;
static constexpr s16 HEIGHT = (s16)32000;

class ScrollingRainbow : public scrolling_map::ScrollingMap {
  private:
	void load_tilesets() override;
	void load_palettes() override;
	ScreenEntry get_tile(s16, s16) override;

  public:
	ScrollingRainbow()
		: ScrollingMap(WIDTH, HEIGHT) {}
};

} // namespace scrolling_rainbow
