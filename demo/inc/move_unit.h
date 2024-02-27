#pragma once

#include "hexes.h"
#include "map.h"

#include <span>
#include <vector>

extern "C" {
#include <tonc_types.h>
}

namespace move_unit {

using hexes::CubeCoord;

void update_palette_of_tile(CubeCoord const, u8);
void update_palettes_of(std::span<CubeCoord const>, u8);

class MoveUnit : public map::Map {
	std::vector<CubeCoord> highlights{};

  public:
	void restore() override;
	void suspend() override;
	void update() override;
};

} // namespace move_unit
