#pragma once

#include "hexes.h"
#include "map.h"
#include "point.h"
#include "state.h"
#include "unit.h"

#include "set.h"
#include <vector>

namespace map {

using hexes::CubeCoord;
using point::Point;
using unit::Unit;

void update_palette_of_tile(CubeCoord const, u8);
void update_palettes_of(Set<CubeCoord> const &, u8);
void deselect();

enum class MapState {
	WaitingForInput,
	Animating,
	SelectingEnemy,
	EnemyTurn,
	AnimatingEnemy,
};

class Map : public state::Mode {
  public:
	u8 animation_cycle = 0;

  private:
	void selected_input();
	void end_player_turn();
	void end_enemy_turn();

	void animation_handler();
	void waiting_for_input_handler();
	void selecting_enemy_handler();
	void enemy_turn_handler();
	void animating_enemy_handler();

	void suspend() override;
	void restore() override;
	void vsync_hook() override;
	bool blackout() override;

  public:
	MapState state = MapState::WaitingForInput;
	size_t enemy_selection = 0;

	void update() override;
};

} // namespace map
