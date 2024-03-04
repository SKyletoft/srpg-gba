#pragma once

#include "hexmap.h"
#include "point.h"
#include <cstring>

namespace hl_map {

using point::Point;
using scrolling_map::Layer;
using tiles::ScreenEntry;

class HighlightMap : public hexmap::Hexmap {
  public:
	ScreenEntry get_tile(Layer &, s16, s16) override;

	HighlightMap(mdspan::Span2d<const u8> const map)
		: Hexmap(map) {}

	HighlightMap &operator=(HighlightMap const &&rhs) {
		std::memcpy((void *)this, (void *)&rhs, sizeof(HighlightMap));
		return *this;
	}
};

} // namespace hl_map
