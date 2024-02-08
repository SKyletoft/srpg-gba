#include "scrolling_hexgrid.h"

#include "hex-overlay.h"
#include "tiles.h"

extern "C" {
#include "dry.h"
#include "grass.h"

#include <tonc.h>
#include <tonc_input.h>
#include <tonc_memdef.h>
#include <tonc_memmap.h>
}

#define memcpy_(dest, src) std::memcpy((void *)dest, src, sizeof(src))

namespace scrolling_hexgrid {

void ScrollingHexgrid::load_tilesets(Layer &layer) {
	// memcpy_(tiles::CHARBLOCKS[BG0_TILE_SOURCE], grassTiles);
	// memcpy_(tiles::SCREENBLOCKS[BG0_TILE_MAP], grassMap);

	STile *tiles = tiles::CHARBLOCKS[layer.tile_source];
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

void ScrollingHexgrid::load_palettes(Layer &) {
	((tiles::Palette *)pal_bg_mem)[0] =
		{.colours = {
			 tiles::TRANSPARENT,
			 tiles::RED,
		 }};
}

ScreenEntry ScrollingHexgrid::get_tile(Layer const &layer, s16 x, s16 y) {
	auto const transparent = ScreenEntry(0, 0, 0);

	if (layer != this->layer0) {
		return transparent;
	}

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

void ScrollingHexgrid::update() { this->ScrollingMap::update(); }

void ScrollingHexgrid::always_update() { this->ScrollingMap::always_update(); }

void ScrollingHexgrid::restore() { this->ScrollingMap::restore(); }

void ScrollingHexgrid::suspend() { this->ScrollingMap::suspend(); }

void ScrollingHexgrid::vsync_hook() { this->ScrollingMap::vsync_hook(); }

} // namespace scrolling_hexgrid
