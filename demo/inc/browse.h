#pragma once

#include "hexes.h"
#include "map.h"

#include "set.h"

namespace browse {

using hexes::CubeCoord;

void update_palette_of_tile(CubeCoord const, u8);
void update_palettes_of(Set<CubeCoord> const &, u8);
void deselect();

class DefaultMap : public map::Map {
  public:
	void update() override;
};

} // namespace browse
