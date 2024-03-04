#include "battle.h"

#include "audio.h"
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

namespace config {
extern u32 sfx_swoosh;
extern u32 sfx_fwoop;
extern u32 jingle_battle;
} // namespace config

namespace battle {

namespace rv = std::ranges::views;

using tiles::Colour;
using tiles::Palette;

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
	u8 x_l = util::lerp(x_l_to, x_l_from, progress);
	u8 x_r = util::lerp(x_r_to, x_r_from, progress);

	this->left.x = x_l;
	this->left.tile_index = frame_l * 64;
	this->right.x = x_r;
	this->right.tile_index = frame_r * 64;
}

void Battle::fight() {
	start_battle_bgm();
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
	if (this->frame == 3 && this->time == 0) {
		audio::play_sfx(config::sfx_swoosh);
	}

	if (this->frame == 6 && this->time == 0) {
		audio::play_sfx(config::sfx_fwoop);
	}

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
	this->left.hide(1);
	this->right.hide(2);
	stop_battle_bgm();

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

		if (!config::used.contains(&container[idx])) {
			config::used.erase(unit);
		}
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

void start_battle_bgm() {
	audio::pause_bgm();
	audio::set_jingle_volume(512);
	audio::play_jingle(config::jingle_battle);
}

void stop_battle_bgm() {
	audio::stop_jingle();
	audio::resume_bgm();
}

} // namespace battle
