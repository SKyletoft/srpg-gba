#pragma once

#include "map.h"

#include <span>
#include <vector>

extern "C" {
#include <tonc_types.h>
}

namespace move_unit {

void update_palettes_of(
	std::span<hexes::CubeCoord const> highlights, u8 new_palette
);

class MoveUnit : public map::Map {
  public:
	void update() override;
};

} // namespace move_unit
