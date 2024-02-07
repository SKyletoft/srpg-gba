#pragma once

#include "scrolling_map.h"
#include "tiles.h"

extern "C" {
#include <tonc_types.h>
}

namespace scrolling_rainbow {

using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;

static constexpr s16 WIDTH = (s16)32000;
static constexpr s16 HEIGHT = (s16)32000;

class ScrollingRainbow : public scrolling_map::ScrollingMap {
  protected:
	void load_tilesets(size_t) override;
	void load_palettes(size_t) override;
	ScreenEntry get_tile(size_t, s16, s16) override;

  public:
	ScrollingRainbow()
		: ScrollingMap(WIDTH, HEIGHT) {}
};

} // namespace scrolling_rainbow
