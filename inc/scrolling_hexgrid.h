#pragma once

#include "scrolling_map.h"
#include "tiles.h"

extern "C" {
#include <tonc_types.h>
}

namespace scrolling_hexgrid {

using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;
using Layer = scrolling_map::Layer;

static constexpr s16 WIDTH = 40 * 3 * 8;
static constexpr s16 HEIGHT = 30 * 4 * 8;

class ScrollingHexgrid : public scrolling_map::ScrollingMap {
  protected:
	void load_tilesets(Layer &) override;
	void load_palettes(Layer &) override;
	ScreenEntry get_tile(Layer const &, s16, s16) override;

  public:
	ScrollingHexgrid()
		: ScrollingMap(WIDTH, HEIGHT) {}
};

} // namespace scrolling_hexgrid
