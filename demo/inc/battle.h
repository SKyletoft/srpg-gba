#pragma once

#include "sprite.h"
#include "state.h"
#include "unit.h"

#include <random>

namespace battle {

using sprite::HardwareSprite;
using unit::Unit;

constexpr std::array<std::tuple<u8, u8, u8, u8>, 10> animation_sequence{
	std::tuple<u8, u8, u8, u8>{48, 0, 128, 0},
	{50, 1, 128, 0},
	{70, 2, 128, 0},
	{90, 3, 128, 0},
	{108, 4, 128, 0},
	{50, 0, 128, 1},
	{50, 0, 108, 2},
	{50, 0, 88, 3},
	{50, 0, 78, 4},
	{50, 0, 128, 0},
};

class Battle : public state::Mode {
	std::default_random_engine rng;

	Unit *left_unit = nullptr;
	Unit *right_unit = nullptr;

	bool continue_to_second_round;

	u8 frame = 0;
	u32 time = 0;

	HardwareSprite left{
		.y = 50,
		.object_mode = sprite::ObjectMode::Normal,
		.x = 20,
		.size = 0b0011,
		.tile_index = 1,
		.palette = 0,
	};
	HardwareSprite right{
		.y = 50,
		.object_mode = sprite::ObjectMode::Normal,
		.x = 150,
		.horizontal_flip = true,
		.size = 0b0011,
		.tile_index = 1,
		.palette = 1,
	};

	static constexpr decltype(time) speed = 8;

  public:
	Battle() : rng() {}

	void update() override;
	void restore() override;
	void suspend() override;
	void vsync_hook() override;
	bool blackout() override;

	void set_combatants(Unit &, Unit &);
	void animation_update();
	void fight();
};

void start_battle_bgm();
void stop_battle_bgm();

} // namespace battle
