#include "map.h"
#include "hex-overlay.h"
#include "state.h"

#include <cstring>

extern "C" {
#include <tonc.h>
#include <tonc_input.h>

#include "arrow.h"
#include "custom-fe7.h"
#include "fe7.h"
}

#define memcpy_(dest, src) std::memcpy(dest, src, sizeof(src))

namespace map {

void MapMode::load_metr_pal() { memcpy_(pal_obj_mem, arrowPal); }

void MapMode::load_metr_data() {
	memcpy_(tiles::CHARBLOCKS[SPRITE_SOURCE].raw, arrowTiles);

	oam_init(obj_buffer, 128);

	ObjectAttribute *cur = &obj_buffer[0];

	const u16 palette = ATTR2_PALBANK(0);
	const u16 tile = 0;
	obj_set_attr(cur, ATTR0_SQUARE, ATTR1_SIZE_16, palette | tile);
}

void MapMode::load_fe8_pal() { memcpy_(pal_bg_mem, fe7Pal); }

void MapMode::load_fe8_data() {
	memcpy_(tiles::CHARBLOCKS[BG0_TILE_SOURCE].raw, fe7Tiles);
	memcpy_(tiles::SCREENBLOCKS[BG0_TILE_MAP].raw, custom_fe7_tilemap);
}

/// Load hex grid from hand written tiles
void MapMode::load_hexgrid() {
	STile *tiles = (STile *)(tile_mem[BG1_TILE_SOURCE]);
	tiles[6] = hex_overlay_3_4::empty;
	tiles[7] = hex_overlay_3_4::hex00;
	tiles[8] = hex_overlay_3_4::hex01;
	tiles[9] = hex_overlay_3_4::hex02;
	tiles[10] = hex_overlay_3_4::hex10;
	tiles[11] = hex_overlay_3_4::hex20;
	tiles[12] = hex_overlay_3_4::hex21;
	tiles[13] = hex_overlay_3_4::hex22;
	tiles[14] = hex_overlay_3_4::hex31;

	auto const transparent = ScreenEntry(6, 0, 0);

	auto const hex00 = ScreenEntry(7, 0, 0);
	auto const hex01 = ScreenEntry(8, 0, 0);
	auto const hex02 = ScreenEntry(9, 0, 0);
	auto const hex10 = ScreenEntry(10, 0, 0);
	auto const hex11 = transparent;
	auto const hex12 = transparent;
	auto const hex20 = ScreenEntry(11, 0, 0);
	auto const hex21 = ScreenEntry(12, 0, 0);
	auto const hex22 = ScreenEntry(13, 0, 0);
	auto const hex30 = transparent;
	auto const hex31 = ScreenEntry(14, 0, 0);
	auto const hex32 = transparent;

	for (size_t j = 0; j < 32; j += 4) {
		auto base0 = &se_mem[BG1_TILE_MAP][j * 32];
		for (size_t i = 0; i < 30; i += 3) {
			base0[i] = hex02;
			base0[i + 1] = hex01;
			base0[i + 2] = hex00;
		}
		base0[30] = hex02;
		base0[31] = hex01;

		auto base1 = &se_mem[BG1_TILE_MAP][(j + 1) * 32];
		for (size_t i = 0; i < 30; i += 3) {
			base1[i] = hex12;
			base1[i + 1] = hex11;
			base1[i + 2] = hex10;
		}
		base1[30] = hex12;
		base1[31] = hex11;

		auto base2 = &se_mem[BG1_TILE_MAP][(j + 2) * 32];
		for (size_t i = 0; i < 30; i += 3) {
			base2[i] = hex22;
			base2[i + 1] = hex21;
			base2[i + 2] = hex20;
		}
		base2[30] = hex22;
		base2[31] = hex21;

		auto base3 = &se_mem[BG1_TILE_MAP][(j + 3) * 32];
		for (size_t i = 0; i < 30; i += 3) {
			base3[i] = hex32;
			base3[i + 1] = hex31;
			base3[i + 2] = hex30;
		}
		base3[30] = hex32;
		base3[31] = hex31;
	}
}

void MapMode::handle_input() {
	switch (key_tri_vert()) {
	case -1: {
		this->y--;
		this->x += (this->y % 2 == 0);
		this->cooldown = COOLDOWN_TIMER;
	} break;
	case 1: {
		this->y++;
		this->x -= (this->y % 2 == 1);
		this->cooldown = COOLDOWN_TIMER;
	} break;
	}
	switch (key_tri_horz()) {
	case -1: {
		this->x--;
		this->cooldown = COOLDOWN_TIMER;
	} break;
	case 1: {
		this->x++;
		this->cooldown = COOLDOWN_TIMER;
	} break;
	}

	if (key_held(1 << KI_R) && key_held(1 << KI_L)) {
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

	load_fe8_pal();
	load_metr_pal();

	REG_BG0CNT = BG_CBB(BG0_TILE_SOURCE) | BG_SBB(BG0_TILE_MAP) | BG_8BPP
				 | BG_REG_64x64 | BG_PRIO(1);
	REG_BG1CNT = BG_CBB(BG1_TILE_SOURCE) | BG_SBB(BG1_TILE_MAP) | BG_4BPP
				 | BG_REG_32x32 | BG_PRIO(0);

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

void MapMode::vsync_hook() { oam_copy(oam_mem, obj_buffer, 1); }

} // namespace map
