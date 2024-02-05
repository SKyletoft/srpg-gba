#include "scrolling_map.h"

#include "util.h"
#include <cstring>
#include <tonc_memdef.h>
#include <tonc_memmap.h>

extern "C" {
#include "dry.h"
#include "grass.h"

#include <tonc.h>
}

#define memcpy_(dest, src) std::memcpy((void *)dest, src, sizeof(src))

namespace scrolling_map {

void load_tilesets() {
	memcpy_(tiles::CHARBLOCKS[BG0_TILE_SOURCE], grassTiles);
	memcpy_(tiles::SCREENBLOCKS[BG0_TILE_MAP], grassMap);
}

void load_palettes() { memcpy_(pal_bg_mem, grassPal); }

void ScrollingMap::load_map() {}

void ScrollingMap::update() {}

void ScrollingMap::always_update() {}

void ScrollingMap::restore() {
	load_tilesets();
	load_map();
	util::wait_for_drawing_start();
	load_palettes();

	REG_BG0CNT =
		BG_CBB(BG0_TILE_SOURCE) | BG_SBB(BG0_TILE_MAP) | BG_4BPP | BG_REG_32x32;
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
}

void ScrollingMap::suspend() {}

void ScrollingMap::vsync_hook() {}

} // namespace scrolling_map
