#pragma once

#include "sprite.h"
#include "state.h"
#include "unit.h"

#include <random>

namespace battle {

using sprite::HardwareSprite;
using unit::Unit;

class Battle : public state::Mode {
	std::default_random_engine rng;

	Unit *left_unit = nullptr;
	Unit *right_unit = nullptr;

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

	static constexpr decltype(time) speed = 5;

  public:
	Battle() : rng() {}

	void update() override;
	void restore() override;
	void vsync_hook() override;
	bool blackout() override;

	void set_combatants(Unit &, Unit &);
};

} // namespace battle
