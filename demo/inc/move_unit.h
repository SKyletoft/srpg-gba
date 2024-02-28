#pragma once

#include "hexes.h"
#include "map.h"

#include "set.h"

extern "C" {
#include <tonc_types.h>
}

namespace move_unit {

using hexes::CubeCoord;

void update_palette_of_tile(CubeCoord const, u8);
void update_palettes_of(Set<CubeCoord> const &, u8);

class MoveUnit : public map::Map {
  public:
	void restore() override;
	void suspend() override;
	void update() override;
};

} // namespace move_unit
