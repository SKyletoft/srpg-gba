#pragma once

#include "scrolling_map.h"

namespace hexmap {

using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;

static constexpr s16 WIDTH = (s16)32000;
static constexpr s16 HEIGHT = (s16)32000;

class Hexmap : public scrolling_map::ScrollingMap {
  protected:
	void load_tilesets(size_t) override;
	void load_palettes(size_t) override;
	ScreenEntry get_tile(size_t, s16, s16) override;

  public:
	Hexmap()
		: ScrollingMap(WIDTH, HEIGHT, 1, 26, 0, 30) {}
};

} // namespace hexmap
