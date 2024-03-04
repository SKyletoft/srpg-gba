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

	HardwareSprite damage_left =
		HardwareSprite{
			.horizontal_flip = false,
			.tile_index = 5 * 64,
			.palette = 2,
		}
			.set_size(sprite::SpriteSize::w32h8);
	HardwareSprite damage_right =
		HardwareSprite{
			.horizontal_flip = false,
			.tile_index = 5 * 64 + 4,
			.palette = 2,
		}
			.set_size(sprite::SpriteSize::w32h8);
	s8 display_hp_left;
	HardwareSprite hp_left =
		HardwareSprite{
			.y = 140,
			.object_mode = sprite::ObjectMode::Normal,
			.x = 150,
			.horizontal_flip = false,
			.tile_index = 5 * 64 + 8,
			.palette = 2,
		}
			.set_size(sprite::SpriteSize::w32h8);
	s8 display_hp_right;
	HardwareSprite hp_right =
		HardwareSprite{
			.y = 140,
			.object_mode = sprite::ObjectMode::Normal,
			.x = 20,
			.horizontal_flip = false,
			.tile_index = 5 * 64 + 12,
			.palette = 2,
		}
			.set_size(sprite::SpriteSize::w32h8);

	static constexpr decltype(time) speed = 8;

  public:
	Battle()
		: rng() {}

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
