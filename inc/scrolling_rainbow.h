#pragma once

#include "scrolling_map.h"
#include "tiles.h"

extern "C" {
#include <tonc_types.h>
}

namespace scrolling_rainbow {

using Layer = scrolling_map::Layer;
using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;

static constexpr s16 WIDTH = (s16)32000;
static constexpr s16 HEIGHT = (s16)32000;

class ScrollingRainbow : public scrolling_map::ScrollingMap {
  protected:
	void load_tilesets(Layer &) override;
	void load_palettes(Layer &) override;
	ScreenEntry get_tile(Layer const &, s16, s16) override;

  public:
	ScrollingRainbow()
		: ScrollingMap(WIDTH, HEIGHT) {}
};

} // namespace scrolling_rainbow
