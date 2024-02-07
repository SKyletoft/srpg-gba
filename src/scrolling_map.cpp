#include "scrolling_map.h"

#include "tiles.h"
#include "util.h"
#include <algorithm>
#include <cstring>

#include "hex-overlay.h"

extern "C" {
#include "dry.h"
#include "grass.h"

#include <tonc.h>
#include <tonc_input.h>
#include <tonc_memdef.h>
#include <tonc_memmap.h>
}

#define memcpy_(dest, src) std::memcpy((void *)dest, src, sizeof(src))

namespace scrolling_map {

void load_tilesets() {
	// memcpy_(tiles::CHARBLOCKS[BG0_TILE_SOURCE], grassTiles);
	// memcpy_(tiles::SCREENBLOCKS[BG0_TILE_MAP], grassMap);

	STile *tiles = tiles::CHARBLOCKS[BG0_TILE_SOURCE];
	tiles[0] = hex_overlay_3_4::empty;
	tiles[1] = hex_overlay_3_4::hex00;
	tiles[2] = hex_overlay_3_4::hex01;
	tiles[3] = hex_overlay_3_4::hex02;
	tiles[4] = hex_overlay_3_4::hex10;
	tiles[5] = hex_overlay_3_4::hex20;
	tiles[6] = hex_overlay_3_4::hex21;
	tiles[7] = hex_overlay_3_4::hex22;
	tiles[8] = hex_overlay_3_4::hex31;
}

void load_palettes() {
	// memcpy_(pal_bg_mem, grassPal);
	*(tiles::Palette *)pal_bg_mem = tiles::Palette{
		.colours =
			{
				tiles::TRANSPARENT,
				tiles::RED,
			}
	};
}

size_t get_screenblock_offset_from_tiles(s16 x, s16 y) {
	while (x < 0) {
		x += 32;
	}
	while (y < 0) {
		y += 32;
	}
	return ((((size_t)y) % 32) * 32) + (((size_t)x) % 32);
}

size_t get_screenblock_offset_from_camera(s16 x, s16 y) {
	return get_screenblock_offset_from_tiles(x / 8, y / 8);
}

ScreenEntry get_tile(s16 x, s16 y) {
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

	ScreenEntry const hexes[4][3] = {
		{hex02, hex01, hex00},
		{hex12, hex11, hex10},
		{hex22, hex21, hex20},
		{hex32, hex31, hex30},
	};

	return hexes[y % 4][x % 3];
}
ScreenEntry get_tile_from_camera(s16 x, s16 y) {
	return get_tile(x / 8, y / 8);
}

void ScrollingMap::load_map() {
	auto base = tiles::SCREENBLOCKS[BG0_TILE_MAP];
	// Yes, load one row too many
	for (s16 x = 0; x <= 30; ++x) {
		for (s16 y = 0; y <= 20; ++y) {
			auto const idx = get_screenblock_offset_from_tiles(x, y);
			base[idx] = get_tile(x, y);
		}
	}
}

void ScrollingMap::update() {
	auto const base = tiles::SCREENBLOCKS[BG0_TILE_MAP];
	auto const f = [=](s16 x, s16 y) {
		size_t const idx = get_screenblock_offset_from_camera(x, y);
		ScreenEntry const tile = get_tile_from_camera(x, y);
		base[idx] = tile;
	};

	s16 const dy = this->y - this->last_load_at_y;
	this->y = std::clamp((s16)(this->y + key_tri_vert()), (s16)0, MAX_Y);
	if (abs(dy) > 4) {
		s16 const diff = dy < 0 ? -4 : 164;
		s16 const cam_y = this->y + diff;

		for (s16 d_cam_x = -8; d_cam_x < 248; d_cam_x += 7) {
			f(this->x + d_cam_x, cam_y);
		}

		this->last_load_at_y = this->y;
	}

	this->x = std::clamp((s16)(this->x + key_tri_horz()), (s16)0, MAX_X);
	s16 const dx = this->x - this->last_load_at_x;
	if (abs(dx) > 4) {
		s16 const diff = dx < 0 ? -4 : 244;
		s16 const cam_x = this->x + diff;
		for (s16 d_cam_y = -8; d_cam_y < 168; d_cam_y += 7) {
			f(cam_x, this->y + d_cam_y);
		}
		this->last_load_at_x = this->x;
	}
}

void ScrollingMap::always_update() {}

void ScrollingMap::restore() {
	load_tilesets();
	load_map();
	util::wait_for_drawing_start();
	load_palettes();

	REG_BG0CNT =
		BG_CBB(BG0_TILE_SOURCE) | BG_SBB(BG0_TILE_MAP) | BG_4BPP | BG_REG_32x32;
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

	REG_BG0HOFS = (u16)this->x;
	REG_BG0VOFS = (u16)this->y;
}

void ScrollingMap::suspend() {
	REG_BG0HOFS = 0;
	REG_BG0VOFS = 0;
}

void ScrollingMap::vsync_hook() {
	REG_BG0HOFS = (u16)this->x;
	REG_BG0VOFS = (u16)this->y;
}

} // namespace scrolling_map
