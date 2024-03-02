#include "browse.h"

#include "hexes.h"
#include "input.h"
#include "state.h"

#include "config.h"

#include <algorithm>
#include <ranges>

namespace browse {

namespace r = std::ranges;
namespace rv = std::ranges::views;

using input::Button;
using input::InputState;
using point::Point;
using tiles::ScreenEntry;

void update_palette_of_tile(CubeCoord const tile, u8 new_palette) {
	auto px = tile.to_pixel_space() - config::hexmap.layer0.pos.into<s32>();
	if (px.x < -24 || 240 < px.x || px.y < -24 || 160 < px.y) {
		return;
	}

	size_t end = 12;
	size_t start = 0;
	// Only draw 2/3 columns for the partially visible tile
	if (px.x == 240) {
		end = 8;
	}
	if (px.x < -16) {
		start = 4;
	}

	auto tile_coord = tile.to_pixel_space() / 8;
	// Stored by column so we can skip the last column when it goes off-screen
	constexpr std::array<size_t, 12> tiles_offsets_in_hex = {
		0, 32, 64, 96, 1, 33, 65, 97, 2, 34, 66, 98
	};

	ScreenEntry volatile *base;
	if (!tile.is_odd()) {
		base = &tiles::SCREENBLOCKS[config::hexmap.layer0.tile_map]
								   [tile_coord.y * 32];
	} else {
		tile_coord.x += 1;
		base = &tiles::SCREENBLOCKS[config::hexmap.layer1.tile_map]
								   [tile_coord.y * 32];
	}

	for (size_t i = start; i < end; ++i) {
		size_t idx = tiles_offsets_in_hex[i] + (size_t)tile_coord.x;
		// This only runs *once*, so not a (% 32) or else only
		// the top line of the hex gets highlighted
		if (idx >= 32) {
			idx -= 32;
		}
		ScreenEntry volatile &se = base[idx];
		ScreenEntry copy = se;
		copy.palette = new_palette & 0b1111;
		se = copy;
	}
}

void update_palettes_of(Set<CubeCoord> const &highlights, u8 new_palette) {
	for (auto const &tile : highlights) {
		update_palette_of_tile(tile, new_palette);
	}
}

void unselected_input() {
	if (input::get_button(Button::A) == InputState::Pressed) {
		auto selected_unit = r::find_if(config::user_units(), [&](auto &unit) {
			return unit.sprite.pos == config::cursor.cursor.pos;
		});

		config::selected_unit = nullptr;
		if (selected_unit != config::user_units().end()
			&& !config::used.contains(&*selected_unit))
		{
			config::selected_unit = &*selected_unit;
			config::highlights =
				config::selected_unit->accessible_tiles(config::hexmap.map);
			update_palettes_of(config::highlights, 1);
			return;
		}
		selected_unit = r::find_if(config::enemy_units(), [&](auto &unit) {
			return unit.sprite.pos == config::cursor.cursor.pos;
		});

		if (selected_unit != config::enemy_units().end()) {
			config::selected_unit = &*selected_unit;
			config::highlights =
				config::selected_unit->accessible_tiles(config::hexmap.map);
			update_palettes_of(config::highlights, 1);
			return;
		}

		state::next_state = 2;
	}
}

void deselect() {
	config::selected_unit = nullptr;
	update_palettes_of(config::highlights, 0);
	config::highlights.clear();
}

void DefaultMap::selected_input() {
	if (input::get_button(Button::B) == InputState::Pressed) {
		config::cursor.cursor.move_to(config::selected_unit->pos());
		deselect();
	}

	if (input::get_button(Button::A) == InputState::Pressed
		&& config::highlights.contains(config::cursor.pos()))
	{
		config::neighbouring_enemies.clear();
		for (auto const &neighbour : hexes::CUBE_DIRECTION_VECTORS) {
			auto const neighbour_ = config::cursor.pos() + neighbour;
			auto const enemy =
				r::find_if(config::enemy_units(), [&](Unit const &enemy) {
					return enemy.pos() == neighbour_;
				});
			if (enemy != config::enemy_units().end()) {
				config::neighbouring_enemies.push_back(&*enemy);
			}
		}

		config::movement_popup.show(0);
		if (config::neighbouring_enemies.size() == 0) {
			config::movement_popup.hide(0);
		}

		state::next_state = 5;
	}
}

void DefaultMap::end_player_turn() {
	for (auto &unit : config::user_units()) {
		unit.sprite.palette = 1;
	}
	config::used.clear();
	config::overlay.is_enemy = true;
	this->state = MapState::EnemyTurn;
	state::next_state = 6;
}

void DefaultMap::end_enemy_turn() {
	for (auto &unit : config::enemy_units()) {
		unit.sprite.palette = 2;
	}
	config::used.clear();
	config::overlay.is_enemy = false;
	this->state = MapState::WaitingForInput;
	state::next_state = 6;
}

void DefaultMap::animation_handler() {
	if (config::selected_unit->sprite.animation == Point<s16>{0, 0}) {
		if (config::neighbouring_enemies.empty()) {
			this->state = MapState::WaitingForInput;
			config::selected_unit->sprite.palette = 3;
			config::used.insert(config::selected_unit);
			config::selected_unit = nullptr;
		} else {
			this->state = MapState::SelectingEnemy;
			config::cursor.cursor.move_to(config::neighbouring_enemies[0]->pos()
			);
		}
	}
}

void DefaultMap::waiting_for_input_handler() {
	if (config::used.size() == config::user_soldier_count) {
		this->end_player_turn();
		return;
	}

	auto const d =
		config::cursor.move_cursor(config::hexmap.layer0.pos.into<s32>());
	config::hexmap.move_in_bounds(d.x, d.y);

	config::hexmap.update_layer_partial(config::hexmap.layer0);
	config::hexmap.update_layer_partial(config::hexmap.layer1);

	if (config::selected_unit != nullptr) {
		if (config::selected_unit->is_user()) {
			selected_input();
			return;
		}
		if (input::get_button(Button::A) == InputState::Pressed
			|| input::get_button(Button::B) == InputState::Pressed)
		{
			deselect();
			return;
		}
	}
	unselected_input();
}

void DefaultMap::selecting_enemy_handler() {
	if (input::get_button(Button::B) == InputState::Pressed) {
		this->state = MapState::WaitingForInput;
		config::selected_unit->sprite.move_to(config::original_pos);
		config::cursor.cursor.move_to(config::selected_unit->pos());
		deselect();
		return;
	}
	if (input::get_button(Button::A) == InputState::Pressed) {
		config::battle_ani.set_combatants(
			*config::selected_unit, config::enemy_units()[this->enemy_selection]
		);
		config::cursor.cursor.move_to(config::selected_unit->pos());
		state::next_state = 4;
		config::selected_unit->sprite.palette = 3;
		config::used.insert(config::selected_unit);
		this->state = MapState::WaitingForInput;
		deselect();
		return;
	}
	if (input::get_button(Button::Up) == InputState::Pressed
		|| input::get_button(Button::Left) == InputState::Pressed)
	{
		if (this->enemy_selection == 0) {
			this->enemy_selection = config::neighbouring_enemies.size();
		}
		this->enemy_selection--;
	}
	if (input::get_button(Button::Down) == InputState::Pressed
		|| input::get_button(Button::Right) == InputState::Pressed)
	{
		this->enemy_selection =
			(this->enemy_selection + 1) % config::neighbouring_enemies.size();
	}
	config::cursor.cursor.move_to(
		config::neighbouring_enemies[this->enemy_selection]->pos()
	);
}

void DefaultMap::enemy_turn_handler() {
	if (config::used.size() == config::enemy_soldier_count) {
		this->end_enemy_turn();
		return;
	}

	for (Unit &enemy : config::enemy_units()) {
		if (config::used.contains(&enemy)) {
			continue;
		}

		Set<CubeCoord> const accessible =
			enemy.accessible_tiles(config::hexmap.map);

		std::vector<std::pair<Unit *, CubeCoord>> vec =
			enemy.attackable_units(accessible);

		auto const f = [&](std::pair<Unit *, CubeCoord> target) {
			enemy.sprite.move_to(target.second);
			config::used.insert(&enemy);
			this->state = MapState::AnimatingEnemy;
			update_palettes_of(accessible, 0);
		};

		switch (vec.size()) {
		case 0:
			config::used.insert(&enemy);
			update_palettes_of(accessible, 0);
			continue;
		case 1:
			f(vec[0]);
			return;
		default:
			auto target = *r::min_element(vec, [&](auto l, auto r) {
				return l.first->stats.health < r.first->stats.health;
			});
			f(target);
			return;
		}
	}
}

void DefaultMap::animating_enemy_handler() {
	if (config::selected_unit->sprite.animation == Point<s16>{0, 0}) {
		end_enemy_turn();
	}
}

void DefaultMap::update() {
	this->animation_cycle = (u8)((this->animation_cycle + 1) % 1024);

	// Terminal on both shoulders
	if (input::get_button(Button::R).is_down()
		&& input::get_button(Button::L).is_down())
	{
		state::next_state = 1;
	}

	switch (this->state) {
	case MapState::Animating:
		this->animation_handler();
		break;
	case MapState::WaitingForInput:
		this->waiting_for_input_handler();
		break;
	case MapState::SelectingEnemy:
		this->selecting_enemy_handler();
		break;
	case MapState::EnemyTurn:
		this->enemy_turn_handler();
		break;
	case MapState::AnimatingEnemy:
		this->animating_enemy_handler();
		break;
	}
}

} // namespace browse
