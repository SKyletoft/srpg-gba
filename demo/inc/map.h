#pragma once

#include "hexes.h"
#include "point.h"
#include "state.h"
#include "unit.h"

#include "set.h"
#include <string>
#include <string_view>

namespace map {

using hexes::CubeCoord;
using point::Point;
using unit::Unit;

void update_palette_of_tile(CubeCoord const, u8);
void update_palettes_of(Set<CubeCoord> const &, u8);
void deselect();
void select_unit(Unit *);

enum class MapState {
	WaitingForInput,
	Animating,
	SelectingEnemy,
	EnemyTurn,
	AnimatingEnemy,
};

struct DrawStatus {
	std::string_view name = "";
	std::string hp_text = "";
	size_t actual_width = 0;
	u16 portrait = 0;

	bool rendered = false;
	bool visible = true;

	constexpr DrawStatus(){};
	DrawStatus(Unit const &unit);

	bool operator==(DrawStatus const &) const;

	void render(size_t ui_layer_map);
};

class Map : public state::Mode {
  private:
	void selected_input();
	void end_player_turn();
	void end_enemy_turn();

	void animation_handler();
	void waiting_for_input_handler();
	void selecting_enemy_handler();
	void enemy_turn_handler();
	void animating_enemy_handler();

  public:
	void suspend() override;
	void restore() override;
	void vsync_hook() override;
	bool blackout() override;
	void update() override;

	u8 animation_cycle = 0;
	size_t const ui_layer_map = 3;
	size_t const ui_layer_source = 2;
	MapState state = MapState::WaitingForInput;
	size_t enemy_selection = 0;
	DrawStatus draw_status{};

};

} // namespace map
