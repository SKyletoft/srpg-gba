#pragma once

#include "hexes.h"
#include "map.h"

#include <span>
#include <vector>

extern "C" {
#include <tonc_types.h>
}

namespace move_unit {

void update_palette_of_tile(hexes::CubeCoord const tile, u8 new_palette);
void update_palettes_of(
	std::span<hexes::CubeCoord const> highlights, u8 new_palette
);

class MoveUnit : public map::Map {
	std::vector<hexes::CubeCoord> highlights{};

  public:
	void restore() override;
	void suspend() override;
	void update() override;
};

} // namespace move_unit
