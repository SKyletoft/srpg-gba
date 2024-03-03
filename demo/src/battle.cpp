#include "battle.h"

#include "config.h"
#include "sprite.h"
#include "state.h"
#include "tiles.h"
#include "util.h"

#include <array>
#include <cstring>
#include <functional>
#include <ranges>
#include <tuple>

extern "C" {
#include <tonc.h>

#include "battle-ani.h"
}

#define END_EARLY()                                                            \
	if (this->left_unit == nullptr || this->right_unit == nullptr) {           \
		state::next_state = 0;                                                 \
		return;                                                                \
	}

namespace battle {

namespace rv = std::ranges::views;

using tiles::Colour;
using tiles::Palette;

// Progress in the range 0-255
u8 lerp(u8 from, u8 to, s32 progress) {
	s32 base = to;
	s32 range = from - to;
	return (u8)(range * progress / 255 + base);
}

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

void Battle::animation_update() {
	this->time++;
	if (this->time > Battle::speed) {
		this->time = 0;
		this->frame = (u8)(this->frame + 1);

		if (this->frame >= animation_sequence.size()) {
			this->time = 0;
			this->frame = 0;
			state::next_state = 0;
		}
	}

	auto [x_l_from, frame_l, x_r_from, frame_r] =
		animation_sequence[this->frame];
	auto [x_l_to, _, x_r_to, __] =
		animation_sequence[(this->frame + 1) % animation_sequence.size()];

	s32 progress = (s32)(this->time * (255 / Battle::speed));
	u8 x_l = lerp(x_l_to, x_l_from, progress);
	u8 x_r = lerp(x_r_to, x_r_from, progress);

	this->left.x = x_l;
	this->left.tile_index = frame_l * 64;
	this->right.x = x_r;
	this->right.tile_index = frame_r * 64;
}

void Battle::fight() {
	std::function attack = [&](Unit &attacker, Unit &defender) {
		s8 damage =
			(s8)std::max(0, attacker.stats.attack - defender.stats.defence);
		defender.stats.health -= damage;
	};

	attack(*this->left_unit, *this->right_unit);
	this->continue_to_second_round = this->right_unit->stats.health > 0;
	if (!this->continue_to_second_round) {
		return;
	}
	attack(*this->right_unit, *this->left_unit);
}

void Battle::update() {
	END_EARLY();
	if (this->frame == 5 && !this->continue_to_second_round) {
		state::next_state = 0;
		return;
	}
	this->animation_update();
}

void Battle::restore() {
	END_EARLY();

	std::memcpy(
		tiles::SPRITE_CHARBLOCK[0], battle_aniTiles, sizeof(battle_aniTiles)
	);

	constexpr Palette RED{
		tiles::TRANSPARENT,
		Colour(5, 0, 0),
		Colour(15, 0, 0),
		Colour(15, 15, 15),
		Colour(31, 0, 0),
		Colour(25, 25, 25),
	};
	constexpr Palette BLUE{
		tiles::TRANSPARENT,
		Colour::from_24bit_colour(0x18, 0x34, 0x97),
		Colour::from_24bit_colour(0x1C, 0x71, 0xD8),
		Colour(15, 15, 15),
		Colour::from_24bit_colour(0x00, 0xBD, 0xEA),
		Colour(25, 25, 25),
	};

	if (config::user_army.begin() <= this->left_unit
		&& this->left_unit < config::user_army.end())
	{
		tiles::SPRITE_PALETTE_MEMORY[0] = BLUE;
	} else {
		tiles::SPRITE_PALETTE_MEMORY[0] = RED;
	}

	if (config::user_army.begin() <= this->right_unit
		&& this->right_unit < config::user_army.end())
	{
		tiles::SPRITE_PALETTE_MEMORY[1] = BLUE;
	} else {
		tiles::SPRITE_PALETTE_MEMORY[1] = RED;
	}

	for (auto i : rv::iota(0uz, 128uz)) {
		HardwareSprite::hide(i);
	}

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;

	this->fight();
}

void Battle::suspend() {
	REG_DISPCNT &= (u16) ~(DCNT_OBJ | DCNT_OBJ_1D);

	auto const maybe_kill = [](Unit *unit) {
		if (unit->stats.health > 0) {
			return;
		}

		auto container = unit->is_user() ? config::user_army.data()
										 : config::enemy_army.data();
		auto &idx = unit->is_user() ? config::user_soldier_count
									: config::enemy_soldier_count;

		idx--;
		std::swap(*unit, container[idx]);
	};

	maybe_kill(this->left_unit);
	maybe_kill(this->right_unit);
}

void Battle::vsync_hook() {
	END_EARLY();
	this->left.write_to_screen(1);
	this->right.write_to_screen(2);
}

bool Battle::blackout() { return false; }

void Battle::set_combatants(Unit &left, Unit &right) {
	this->left_unit = &left;
	this->right_unit = &right;
}

} // namespace battle
