#pragma once

#include "hexes.h"
#include "map.h"
#include "unit.h"

#include "set.h"
#include <vector>

namespace browse {

using hexes::CubeCoord;
using unit::Unit;

void update_palette_of_tile(CubeCoord const, u8);
void update_palettes_of(Set<CubeCoord> const &, u8);
void deselect();

enum class MapState {
	WaitingForInput,
	Animating,
	SelectingEnemy,
};

class DefaultMap : public map::Map {
  private:
	void selected_input();

  public:
	MapState state = MapState::WaitingForInput;
	size_t enemy_selection = 0;

	void update() override;
};

} // namespace browse
