#pragma once

#include "hexmap.h"
#include "point.h"

namespace hl_map {

using scrolling_map::Layer;
using tiles::ScreenEntry;
using point::Point;

class HighlightMap : public hexmap::Hexmap {
  public:
	ScreenEntry get_tile(Layer &, s16, s16) override;

	HighlightMap(
		std::array<std::array<u8, hexmap::WIDTH>, hexmap::HEIGHT> const &map
	)
		: Hexmap(map) {}
};

} // namespace hl_map
