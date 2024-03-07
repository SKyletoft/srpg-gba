#include "map.h"

#include "audio.h"
#include "hexes.h"
#include "image.h"
#include "input.h"
#include "sprite.h"
#include "state.h"
#include "tiles.h"
#include "util.h"

#include "config.h"
#include "loading.h"
#include "unit.h"

#include <cstring>
#include <format>
#include <ranges>

extern "C" {
#include "arrow.h"
}

namespace map {

namespace r = std::ranges;
namespace rv = std::ranges::views;

using input::Button;
using input::InputState;
using point::Point;
using tiles::ScreenEntry;

void maybe_end_game() {
	if (config::enemy_soldier_count == 0) {
		config::image.bg = image::Background::Win;
		state::next_state = 3;
	}
	if (config::user_soldier_count == 0) {
		config::image.bg = image::Background::GameOver;
		state::next_state = 3;
	}
}

Unit *get_hovered_unit() {
	auto const range =
		std::array{config::user_units(), config::enemy_units()} | rv::join;
	auto const selected_unit = r::find_if(range, [&](auto &unit) {
		return unit.sprite.pos == config::cursor.cursor.pos;
	});
	if (selected_unit == range.end()) {
		return nullptr;
	}
	return &*selected_unit;
}

// Assumes a player unit is selected and so should only be called
// when such is the case
void cycle_selected_unit() {
	Unit *next_unit = config::selected_unit;
	do {
		next_unit++;
		if (next_unit == &*config::user_units().end()) {
			next_unit = &*config::user_units().begin();
		}
		if (config::selected_unit == next_unit) {
			return;
		}
	} while (config::used.contains(next_unit));
	deselect();
	config::cursor.cursor.move_to(next_unit->pos());
	audio::play_sfx(SFX__BLIP);
	select_unit(next_unit);
	return;
}

void cycle_hovered_unit() {
	Unit *next_unit = get_hovered_unit();
	if (next_unit == nullptr) {
		next_unit = &*config::user_units().begin() - 1;
	}

	do {
		next_unit++;
		if (next_unit->is_user()) {
			if (next_unit == &*config::user_units().end()) {
				next_unit = &*config::user_units().begin();
			}
		} else {
			if (next_unit == &*config::enemy_units().end()) {
				next_unit = &*config::enemy_units().begin();
			}
		}
	} while (config::used.contains(next_unit));
	config::cursor.cursor.move_to(next_unit->pos());
	audio::play_sfx(SFX__BLIP);
}

DrawStatus::DrawStatus(Unit const &unit)
	: name(unit.name)
	, hp_text(std::format("{}/{}", unit.stats.health, unit.stats.max_health))
	, actual_width(
		  std::min(std::max(unit.name.size(), hp_text.size()) + 6, 30uz)
	  )
	, portrait(unit.portrait)
	, rendered(false) {}

bool DrawStatus::operator==(DrawStatus const &rhs) const {
	// Explicitly don't check rendered or visible
	return this->name == rhs.name && hp_text == rhs.hp_text
		   && this->actual_width == rhs.actual_width
		   && this->portrait == rhs.portrait;
}

void DrawStatus::render(size_t ui_layer_map) {
	auto const idx = [](size_t x, size_t y) { return y * 32 + x; };

	for (size_t x = 0; x < this->actual_width; ++x) {
		for (size_t y = 0; y < 4; ++y) {
			tiles::SCREENBLOCKS[ui_layer_map][idx(x, y)] =
				ScreenEntry(2, 0, 15);
		}
	}
	for (size_t x = this->actual_width; x < 30; ++x) {
		for (size_t y = 0; y < 4; ++y) {
			tiles::SCREENBLOCKS[ui_layer_map][idx(x, y)] =
				ScreenEntry(0, 0, 15);
		}
	}
	for (auto [x, c] : this->name | rv::enumerate | rv::take(25)) {
		tiles::SCREENBLOCKS[ui_layer_map][idx((size_t)x + 5, 1)] =
			ScreenEntry((u16)(c - ' ' + 2), 0, 15);
	}
	for (auto [x, c] : hp_text | rv::enumerate | rv::take(25)) {
		tiles::SCREENBLOCKS[ui_layer_map][idx((size_t)x + 5, 2)] =
			ScreenEntry((u16)(c - ' ' + 2), 0, 15);
	}

	constexpr u16 PORTRAIT_BASE_OFFSET = 98;
	auto const offset = [&](u16 offset) {
		return (u16)(PORTRAIT_BASE_OFFSET + this->portrait * 4 + offset);
	};
	tiles::SCREENBLOCKS[ui_layer_map][33] = ScreenEntry(offset(0), 0, 14);
	tiles::SCREENBLOCKS[ui_layer_map][34] = ScreenEntry(offset(1), 0, 14);
	tiles::SCREENBLOCKS[ui_layer_map][65] = ScreenEntry(offset(2), 0, 14);
	tiles::SCREENBLOCKS[ui_layer_map][66] = ScreenEntry(offset(3), 0, 14);

	this->rendered = true;
}

void Map::suspend() { config::cursor.cursor.hide(); }

void Map::restore() {
	if (state::blacked_out) {
		loading::load_all();
	}

	switch (state::last_state) {
	case 0:
	case 2:
	case 5:
	case 11: {
	} break;
	case 1:
	case 7:
	case 8:
	case 9: {
		for (auto i : rv::iota(0uz, 128uz)) {
			sprite::HardwareSprite::hide(i);
		}

		loading::load_sprites();
		loading::load_palettes();
		loading::load_tiles();
		loading::load_map();
		loading::load_ui();
	} break;
	case 4: {
		loading::load_sprites();
		loading::load_ui();
	} break;
	case 6: {
		loading::load_ui();
	} break;
	case 10: {
		loading::load_palettes();
		loading::load_tiles();
	} break;
	default:
		break;
	}

	if (state::last_state != 2) {
		tiles::SPRITE_PALETTE_MEMORY[0] = *(tiles::Palette *)arrowPal;
	}

	std::memcpy(&tiles::SPRITE_CHARBLOCK[0][1], arrowTiles, sizeof(arrowTiles));

	config::cursor.cursor.animation = {0, 0};
	config::cursor.cursor.render(config::hexmap.layer0.pos);

	util::clear_layer(this->ui_layer_map);

	REG_BG0CNT = (u16)(BG_CBB(config::hexmap.layer0.tile_source)
					   | BG_SBB(config::hexmap.layer0.tile_map) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(3));
	REG_BG1CNT = (u16)(BG_CBB(config::hexmap.layer1.tile_source)
					   | BG_SBB(config::hexmap.layer1.tile_map) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(3));
	REG_BG2CNT = (u16)(BG_CBB(config::map.ui_layer_source)
					   | BG_SBB(config::map.ui_layer_map) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(0));
	REG_DISPCNT =
		DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D;

	this->draw_status = {};
}

void Map::vsync_hook() {
	config::hexmap.update_camera();
	config::cursor.cursor.render(config::hexmap.layer0.pos);

	config::cursor.cursor.animation.x =
		(s16)((config::cursor.cursor.animation.x * (s16)3) / (s16)4);
	config::cursor.cursor.animation.y =
		(s16)((config::cursor.cursor.animation.y * (s16)3) / (s16)4);

	u8 animation_cycle = (u8)(this->animation_cycle / 20);

	for (auto &unit :
		 std::array{config::user_units(), config::enemy_units()} | rv::join)
	{
		unit.sprite.move();
		unit.render(config::hexmap.layer0.pos, animation_cycle);
	}

	if (this->draw_status.visible) {
		REG_BG2VOFS = 0;
	} else {
		REG_BG2VOFS = (u16)(-160);
	}
	if (!this->draw_status.rendered) {
		this->draw_status.render(this->ui_layer_map);
	}
}

bool Map::blackout() { return false; }

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
		base = &tiles::SCREENBLOCKS[config::hexmap.layer0.tile_map][0];
	} else {
		tile_coord.x += 1;
		base = &tiles::SCREENBLOCKS[config::hexmap.layer1.tile_map][0];
	}

	for (size_t i = start; i < end; ++i) {
		size_t idx = tiles_offsets_in_hex[i] + (size_t)tile_coord.x;
		// This only runs *once*, so not a (% 32) or else only
		// the top line of the hex gets highlighted
		if (idx >= 32) {
			idx -= 32;
		}
		idx += (size_t)(tile_coord.y * 32);
		ScreenEntry volatile &se = base[idx % 1024];
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
		select_unit(get_hovered_unit());
	}
	if (input::get_button(Button::L) == InputState::Pressed) {
		cycle_hovered_unit();
	}
}

void deselect() {
	config::selected_unit = nullptr;
	update_palettes_of(config::highlights, 2);
	config::highlights.clear();
}

void select_unit(Unit *selected_unit) {
	if (selected_unit != nullptr) {
		config::selected_unit = &*selected_unit;
		config::highlights =
			config::selected_unit->accessible_tiles(config::hexmap.map);
		update_palettes_of(config::highlights, 1);
		return;
	}
	state::next_state = 2;
}

void Map::selected_input() {
	if (input::get_button(Button::B) == InputState::Pressed) {
		config::cursor.cursor.move_to(config::selected_unit->pos());
		deselect();
	}

	if (input::get_button(Button::A) == InputState::Pressed
		&& config::highlights.contains(config::cursor.pos())
		&& !config::used.contains(config::selected_unit))
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

		this->draw_status.visible = false;
		state::next_state = 5;
	}

	// Assumes a player unit is selected, which should always be the case here?
	if (input::get_button(Button::L) == InputState::Pressed) {
		cycle_selected_unit();
	}
}

void Map::end_player_turn() {
	for (auto &unit : config::user_units()) {
		unit.sprite.palette = 1;
	}
	REG_DISPCNT &= (u16)~DCNT_BG2;
	config::used.clear();
	config::overlay.image = overlay::Image::Enemy;
	config::selected_unit = nullptr;
	this->state = MapState::EnemyTurn;
	config::cursor.cursor.hidden = true;
	state::next_state = 6;
}

void Map::end_enemy_turn() {
	for (auto &unit : config::enemy_units()) {
		unit.sprite.palette = 2;
	}
	config::used.clear();
	config::overlay.image = overlay::Image::Player;
	config::selected_unit = nullptr;
	this->state = MapState::WaitingForInput;
	config::cursor.cursor.hidden = false;
	state::next_state = 6;

	config::cursor.pos() = hexes::CubeCoord::from_axial_coord({-1, -1});
	cycle_hovered_unit();
}

void Map::animation_handler() {
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

void Map::waiting_for_input_handler() {
	maybe_end_game();

	REG_DISPCNT |= (u16)DCNT_BG2;

	if (config::used.size() == config::user_soldier_count) {
		this->end_player_turn();
		return;
	}

	bool const moved =
		config::cursor.move_cursor(config::hexmap.layer0.pos.into<s32>());
	if (moved) {
		audio::play_sfx(SFX__BLIP);
	}
	Point<s16> const diff =
		config::cursor.recentre_camera(config::hexmap.layer0.pos.into<s32>());
	config::hexmap.move_in_bounds(diff.x, diff.y);

	config::hexmap.update_layer_partial(config::hexmap.layer0);
	config::hexmap.update_layer_partial(config::hexmap.layer1);

	if (input::get_button(Button::Start) == InputState::Pressed) {
		state::next_state = 2;
		return;
	}
	if (input::get_button(Button::Select) == InputState::Pressed) {
		for (auto &unit : config::user_units()) {
			config::used.insert(&unit);
		}
		return;
	}

	if (auto hovered_unit = get_hovered_unit()) {
		auto new_status = DrawStatus(*hovered_unit);
		if (new_status != this->draw_status) {
			this->draw_status = new_status;
		}
		this->draw_status.visible = true;

		if (input::get_button(Button::R) == InputState::Pressed) {
			config::stats.set_unit(*hovered_unit);
			state::next_state = 10;
		}
	} else if (config::selected_unit != nullptr) {
		auto new_status = DrawStatus(*config::selected_unit);
		if (new_status != this->draw_status) {
			this->draw_status = new_status;
		}
		this->draw_status.visible = true;

		if (input::get_button(Button::R) == InputState::Pressed) {
			config::stats.set_unit(*config::selected_unit);
			state::next_state = 10;
		}
	} else {
		this->draw_status.visible = false;
	}

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

void Map::selecting_enemy_handler() {
	if (input::get_button(Button::B) == InputState::Pressed) {
		this->state = MapState::WaitingForInput;
		config::selected_unit->sprite.move_to(config::original_pos);
		config::cursor.cursor.move_to(config::selected_unit->pos());
		deselect();
		return;
	}
	if (input::get_button(Button::A) == InputState::Pressed) {
		config::battle_ani.set_combatants(
			*config::selected_unit,
			*config::neighbouring_enemies[this->enemy_selection]
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
	auto new_status =
		DrawStatus(*config::neighbouring_enemies[this->enemy_selection]);
	if (new_status != this->draw_status) {
		this->draw_status = new_status;
	}
	this->draw_status.visible = true;
}

void Map::enemy_turn_handler() {
	maybe_end_game();

	if (config::used.size() >= config::enemy_soldier_count) {
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
			config::selected_unit = &enemy;
			enemy.sprite.move_to(target.second);
			config::cursor.cursor.move_to(target.second);
			this->state = MapState::AnimatingEnemy;
			config::battle_ani.set_combatants(enemy, *target.first);
			update_palettes_of(accessible, 2);
		};

		switch (vec.size()) {
		case 0:
			config::used.insert(&enemy);
			enemy.sprite.palette = 4;
			update_palettes_of(accessible, 2);
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

void Map::animating_enemy_handler() {
	Point<s16> const diff =
		config::cursor.recentre_camera(config::hexmap.layer0.pos.into<s32>());
	config::hexmap.move_in_bounds(diff.x, diff.y);

	config::hexmap.update_layer_partial(config::hexmap.layer0);
	config::hexmap.update_layer_partial(config::hexmap.layer1);
	if (diff != Point<s16>{0, 0}) {
		return;
	}

	if (config::selected_unit == nullptr
		|| config::selected_unit->sprite.animation == Point<s16>{0, 0})
	{
		config::used.insert(config::selected_unit);
		config::selected_unit->sprite.palette = 4;
		config::selected_unit = nullptr;
		this->state = MapState::EnemyTurn;
		state::next_state = 4;
	}
}

void Map::animating_cutscene_handler() {
	Point<s16> const diff =
		config::cursor.recentre_camera(config::hexmap.layer0.pos.into<s32>());
	config::hexmap.move_in_bounds(diff.x, diff.y);

	config::hexmap.update_layer_partial(config::hexmap.layer0);
	config::hexmap.update_layer_partial(config::hexmap.layer1);

	config::cursor.cursor.hidden = false;

	if (diff == Point<s16>{0, 0}) {
		this->animation_pause++;
		if (this->animation_pause > 60) {
			this->state = MapState::WaitingForInput;
			state::next_state = 11;
			config::cursor.cursor.hidden = true;
		}
	}
}

void Map::update() {
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
	case MapState::AnimatingCutscene:
		this->animating_cutscene_handler();
		break;
	}
}

} // namespace map
