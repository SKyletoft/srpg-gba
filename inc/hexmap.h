#pragma once

#include "scrolling_map.h"

namespace hexmap {

using scrolling_map::Layer;
using tiles::ScreenEntry;
using tiles::STile;

static constexpr s16 WIDTH = (s16)32000;
static constexpr s16 HEIGHT = (s16)32000;

class Hexmap : public scrolling_map::ScrollingMap {
  protected:
	void load_tilesets(Layer &) override;
	void load_palettes(Layer &) override;
	ScreenEntry get_tile(Layer &, s16, s16) override;

  public:
	Hexmap()
		: ScrollingMap(WIDTH, HEIGHT, 1, 26, 0, 30) {}
};

} // namespace hexmap
