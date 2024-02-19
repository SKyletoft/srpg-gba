#include "map.h"

#include "hex-overlay.h"
#include "input.h"
#include "state.h"
#include "tiles.h"
#include "util.h"
#include <cstring>

extern "C" {
#include <tonc.h>
#include <tonc_video.h>

#include "arrow.h"
#include "custom-fe7.h"
#include "fe7.h"
}

#define memcpy_(dest, src) std::memcpy((void *)dest, src, sizeof(src))

namespace map {

using input::Button;

void MapMode::load_metr_pal() { memcpy_(pal_obj_mem, arrowPal); }

void MapMode::load_metr_data() {
	memcpy_(tiles::CHARBLOCKS[SPRITE_SOURCE], arrowTiles);

	oam_init(this->obj_buffer, 128);

	ObjectAttribute *cur = &this->obj_buffer[0];

	const u16 palette = ATTR2_PALBANK(0);
	const u16 tile = 0;
	obj_set_attr(cur, ATTR0_SQUARE, ATTR1_SIZE_16, palette | tile);
}

void MapMode::load_fe8_pal() { memcpy_(pal_bg_mem, fe7Pal); }

void MapMode::load_fe8_data() {
	memcpy_(tiles::CHARBLOCKS[BG0_TILE_SOURCE], fe7Tiles);
	memcpy_(tiles::SCREENBLOCKS[BG0_TILE_MAP], custom_fe7_tilemap);
}

/// Load hex grid from hand written tiles
void MapMode::load_hexgrid() {
	STile *tiles = tiles::CHARBLOCKS[BG1_TILE_SOURCE];
	tiles[0] = hex_overlay_3_4::empty;
	tiles[1] = hex_overlay_3_4::hex00;
	tiles[2] = hex_overlay_3_4::hex01;
	tiles[3] = hex_overlay_3_4::hex02;
	tiles[4] = hex_overlay_3_4::hex10;
	tiles[5] = hex_overlay_3_4::hex20;
	tiles[6] = hex_overlay_3_4::hex21;
	tiles[7] = hex_overlay_3_4::hex22;
	tiles[8] = hex_overlay_3_4::hex31;

	auto const transparent = ScreenEntry(0, 0, 0);

	auto const hex00 = ScreenEntry(1, 0, 0);
	auto const hex01 = ScreenEntry(2, 0, 0);
	auto const hex02 = ScreenEntry(3, 0, 0);
	auto const hex10 = ScreenEntry(4, 0, 0);
	auto const hex11 = transparent;
	auto const hex12 = transparent;
	auto const hex20 = ScreenEntry(5, 0, 0);
	auto const hex21 = ScreenEntry(6, 0, 0);
	auto const hex22 = ScreenEntry(7, 0, 0);
	auto const hex30 = transparent;
	auto const hex31 = ScreenEntry(8, 0, 0);
	auto const hex32 = transparent;

	for (size_t j = 0; j < 32; j += 4) {
		auto base = &tiles::SCREENBLOCKS[BG1_TILE_MAP][j * 32];
		for (size_t i = 0; i < 30; i += 3) {
			base[i] = hex02;
			base[i + 1] = hex01;
			base[i + 2] = hex00;
		}
		base[30] = hex02;
		base[31] = hex01;

		base += 32;
		for (size_t i = 0; i < 30; i += 3) {
			base[i] = hex12;
			base[i + 1] = hex11;
			base[i + 2] = hex10;
		}
		base[30] = hex12;
		base[31] = hex11;

		base += 32;
		for (size_t i = 0; i < 30; i += 3) {
			base[i] = hex22;
			base[i + 1] = hex21;
			base[i + 2] = hex20;
		}
		base[30] = hex22;
		base[31] = hex21;

		base += 32;
		for (size_t i = 0; i < 30; i += 3) {
			base[i] = hex32;
			base[i + 1] = hex31;
			base[i + 2] = hex30;
		}
		base[30] = hex32;
		base[31] = hex31;
	}
}

void MapMode::handle_input() {
	if (input::get_button(Button::Up).is_down()) {
		this->y--;
		this->x += (this->y % 2 == 0);
		this->cooldown = COOLDOWN_TIMER;
	}
	if (input::get_button(Button::Down).is_down()) {
		this->y++;
		this->x -= (this->y % 2 == 1);
		this->cooldown = COOLDOWN_TIMER;
	}
	if (input::get_button(Button::Left).is_down()) {
		this->x--;
		this->cooldown = COOLDOWN_TIMER;
	}
	if (input::get_button(Button::Right).is_down()) {
		this->x++;
		this->cooldown = COOLDOWN_TIMER;
	}

	if (input::get_button(Button::R).is_down()
		&& input::get_button(Button::L).is_down())
	{
		state::next_state = 1;
	}
}

void MapMode::always_update() {}

void MapMode::update() {
	if (this->cooldown < 0) {
		this->handle_input();
	}

	this->cooldown--;
	this->t = (this->t + 10) % 512;

	int actual_x = this->x * 8 * 3 + 4 + 12 * (this->y % 2);
	int actual_y = this->y * 8 * 2 + sin_lut[this->t] / 1024 - 5;
	obj_set_pos(this->arrow, actual_x, actual_y);
}

void MapMode::suspend() {}

void MapMode::restore() {
	load_fe8_data();
	load_hexgrid();
	load_metr_data();

	// vid_vsync();
	util::wait_for_vsync();
	load_fe8_pal();
	load_metr_pal();

	REG_BG0CNT = BG_CBB(BG0_TILE_SOURCE) | BG_SBB(BG0_TILE_MAP) | BG_8BPP
				 | BG_REG_64x64 | BG_PRIO(1);
	REG_BG1CNT = BG_CBB(BG1_TILE_SOURCE) | BG_SBB(BG1_TILE_MAP) | BG_4BPP
				 | BG_REG_32x32 | BG_PRIO(0);

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

void MapMode::vsync_hook() { oam_copy(oam_mem, this->obj_buffer, 1); }

} // namespace map
