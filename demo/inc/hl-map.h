#pragma once

#include "hexmap.h"

namespace hl_map {

using scrolling_map::Layer;
using tiles::ScreenEntry;

class HighlightMap : public hexmap::Hexmap {
  public:
	ScreenEntry get_tile(Layer &, s16, s16) override;

	HighlightMap(
		std::array<std::array<u8, hexmap::WIDTH>, hexmap::HEIGHT> const &map
	)
		: Hexmap(map) {}
};

} // namespace hl_map
