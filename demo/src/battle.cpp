#include "battle.h"

#include "audio.h"
#include "config.h"
#include "font.h"
#include "sprite.h"
#include "state.h"
#include "tiles.h"
#include "util.h"

#include <array>
#include <cstring>
#include <format>
#include <functional>
#include <ranges>

extern "C" {
#include <tonc.h>
#include <tonc_tte.h>

#include "battle-ani.h"
}

#define END_EARLY()                                                            \
	if (this->left_unit == nullptr || this->right_unit == nullptr) {           \
		state::next_state = 0;                                                 \
		return;                                                                \
	}

namespace battle {

namespace rv = std::ranges::views;

using hexmap::STile;
using tiles::Colour;
using tiles::Palette;
using tiles::SPRITE_CHARBLOCK;

STile *const POST_ANIMATION_TILES = &SPRITE_CHARBLOCK[0][5 * 64];
STile *const ALPHABET_TILES = POST_ANIMATION_TILES + 16;

void Battle::animation_update() {
	this->time++;
	if (this->time > Battle::speed) {
		this->time = 0;
		this->frame = (u8)(this->frame + 1);

		if (this->frame >= ANIMATION_SEQUENCE.size()) {
			this->time = 0;
			this->frame = 0;
			state::next_state = 0;
		}
	}

	auto [x_l_from, frame_l, x_r_from, frame_r] =
		ANIMATION_SEQUENCE[this->frame];
	auto [x_l_to, _, x_r_to, __] =
		ANIMATION_SEQUENCE[(this->frame + 1) % ANIMATION_SEQUENCE.size()];

	s32 progress = (s32)(this->time * (255 / Battle::speed));
	u8 x_l = util::lerp(x_l_to, x_l_from, progress);
	u8 x_r = util::lerp(x_r_to, x_r_from, progress);

	this->left.x = x_l;
	this->left.tile_index = frame_l * 64;
	this->right.x = x_r;
	this->right.tile_index = frame_r * 64;
}

void write_number(STile *tile, s8 num) {
	num = (s8)std::clamp((s32)num, 0, 9999);
	std::string damage_text = std::format("{:4}", num);
	for (size_t i = 0; i < 4; ++i) {
		tile[i] = ALPHABET_TILES[damage_text[i] - ' '];
	}
}

void Battle::fight() {
	start_battle_bgm();
	std::function attack = [&](Unit &attacker, Unit &defender, STile *tile) {
		s8 damage = s8(
			std::clamp(attacker.stats.attack - defender.stats.defence, 0, 9999)
		);
		write_number(tile, damage);
		defender.stats.health -= damage;
	};

	this->display_hp_left = this->left_unit->stats.health;
	this->display_hp_right = this->right_unit->stats.health;

	attack(*this->left_unit, *this->right_unit, POST_ANIMATION_TILES);
	this->continue_to_second_round = this->right_unit->stats.health > 0;
	if (!this->continue_to_second_round) {
		return;
	}
	attack(*this->right_unit, *this->left_unit, POST_ANIMATION_TILES + 4);
}

void Battle::update() {
	END_EARLY();

	if (this->frame == 3 && this->time == 0) {
		audio::play_sfx(config::sfx_swoosh);
		this->damage_right.object_mode = sprite::ObjectMode::Normal;
		this->damage_right.x = 130;
		this->damage_right.y = 30;
	}

	if (this->frame == 3) {
		this->damage_right.palette = this->time % 2 ? 2 : 3;
	}
	if (this->frame > 3 && this->time % 4 == 0
		&& this->display_hp_right > this->right_unit->stats.health)
	{
		this->display_hp_right--;
	}
	if (this->frame == 6 && this->time == 0) {
		this->damage_right.object_mode = sprite::ObjectMode::Hidden;
	}

	if (this->frame == 6 && this->time == 0) {
		audio::play_sfx(config::sfx_fwoop);
		this->damage_left.object_mode = sprite::ObjectMode::Normal;
		this->damage_left.x = 50;
		this->damage_left.y = 30;
	}

	if (this->frame == 6) {
		this->damage_left.palette = this->time % 2 ? 2 : 3;
	}
	if (this->frame > 6 && this->time % 4 == 0
		&& this->display_hp_left > this->left_unit->stats.health)
	{
		this->display_hp_left--;
	}
	if (this->frame == 9 && this->time == 0) {
		this->damage_left.object_mode = sprite::ObjectMode::Hidden;
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
	// tty::decompress_1bpp_to_4bpp(alphabet_tiles, sys8Glyphs, '9' - ' ' + 1);
	std::memcpy(ALPHABET_TILES, fontTiles + 8, fontTilesLen - 8 * sizeof(int));

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
	constexpr Palette WHITE{tiles::TRANSPARENT, tiles::WHITE, tiles::BLACK};
	constexpr Palette BLACK{tiles::TRANSPARENT, tiles::BLACK, tiles::WHITE};

	tiles::SPRITE_PALETTE_MEMORY[2] = WHITE;
	tiles::SPRITE_PALETTE_MEMORY[3] = BLACK;

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
	this->damage_left.object_mode = sprite::ObjectMode::Hidden;
	this->damage_right.object_mode = sprite::ObjectMode::Hidden;

	this->hp_left.write_to_screen(5);
	this->hp_right.write_to_screen(6);

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;

	this->frame = 0;
	this->time = 0;

	this->fight();
}

void Battle::suspend() {
	HardwareSprite::hide(1);
	HardwareSprite::hide(2);
	HardwareSprite::hide(3);
	HardwareSprite::hide(4);
	HardwareSprite::hide(5);
	HardwareSprite::hide(6);

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
	this->damage_left.write_to_screen(3);
	this->damage_right.write_to_screen(4);

	write_number(POST_ANIMATION_TILES + 8, this->display_hp_right);
	write_number(POST_ANIMATION_TILES + 12, this->display_hp_left);
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
