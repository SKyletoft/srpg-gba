#include "scrolling_map.h"

#include "tiles.h"
#include "util.h"
#include <algorithm>
#include <cstring>

extern "C" {
#include <tonc.h>
#include <tonc_input.h>
#include <tonc_memdef.h>
#include <tonc_memmap.h>
}

#define memcpy_(dest, src) std::memcpy((void *)dest, src, sizeof(src))

namespace scrolling_map {

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

ScreenEntry ScrollingMap::get_tile_from_camera(size_t layer, s16 x, s16 y) {
	return this->get_tile(layer, x / 8, y / 8);
}

void ScrollingMap::load_map(size_t layer) {
	auto base = tiles::SCREENBLOCKS[layer];
	// Yes, load one row too many
	for (s16 x = 0; x <= 30; ++x) {
		for (s16 y = 0; y <= 20; ++y) {
			auto const idx = get_screenblock_offset_from_tiles(x, y);
			base[idx] = this->get_tile(layer, x, y);
		}
	}
}

void ScrollingMap::update_layer(size_t layer) {
	ScreenEntry volatile *const base = tiles::SCREENBLOCKS[layer];
	auto const f = [&](s16 x, s16 y) {
		size_t const idx = get_screenblock_offset_from_camera(x, y);
		ScreenEntry const tile = this->get_tile_from_camera(layer, x, y);
		base[idx] = tile;
	};

	s16 const dy = this->y - this->last_load_at_y;
	this->y = std::clamp((s16)(this->y + key_tri_vert()), (s16)0, this->max_y);
	if (abs(dy) > 4) {
		s16 const diff = dy < 0 ? -4 : 164;
		s16 const cam_y = this->y + diff;

		for (s16 d_cam_x = -8; d_cam_x < 248; d_cam_x += 7) {
			f(this->x + d_cam_x, cam_y);
		}

		this->last_load_at_y = this->y;
	}

	this->x = std::clamp((s16)(this->x + key_tri_horz()), (s16)0, this->max_x);
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

void ScrollingMap::update() { this->update_layer(this->bg0_tile_map); }

void ScrollingMap::always_update() {}

void ScrollingMap::restore() {
	this->load_tilesets(this->bg0_tile_source);
	// this->load_tilesets(this->bg1_tile_source);
	this->load_map(this->bg0_tile_map);
	// this->load_map(this->bg1_tile_map);
	util::wait_for_drawing_start();
	this->load_palettes(0);

	REG_BG0CNT = BG_CBB((u16)this->bg0_tile_source)
				 | BG_SBB((u16)this->bg0_tile_map) | BG_4BPP | BG_REG_32x32;
	// REG_BG1CNT = BG_CBB((u16)this->bg1_tile_source)
	//			 | BG_SBB((u16)this->bg1_tile_map) | BG_4BPP | BG_REG_32x32;
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
