#pragma once

#include "scrolling_map.h"

namespace hexmap {

using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;
using scrolling_map::Layer;

static constexpr s16 WIDTH = (s16)32000;
static constexpr s16 HEIGHT = (s16)32000;

class Hexmap : public scrolling_map::ScrollingMap {
  protected:
	void load_tilesets(Layer &) override;
	void load_palettes(Layer &) override;
	ScreenEntry get_tile(Layer const &, s16, s16) override;

  public:
	Hexmap()
		: ScrollingMap(WIDTH, HEIGHT, 1, 26, 0, 30) {}
};

} // namespace hexmap
