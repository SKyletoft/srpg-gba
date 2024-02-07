#pragma once

#include "state.h"
#include "tiles.h"

extern "C" {
#include <tonc_types.h>
}

namespace scrolling_map {

using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;

size_t get_screenblock_offset_from_tiles(s16 x, s16 y);
size_t get_screenblock_offset_from_camera(s16 x, s16 y);

class ScrollingMap : public state::Mode {
  protected:
	const size_t bg0_tile_source = 0;
	const size_t bg0_tile_map = 30;
	const size_t bg1_tile_source = 1;
	const size_t bg1_tile_map = 29;

	const s16 max_x = 40 * 3 * 8;
	const s16 max_y = 30 * 4 * 8;
	s16 x = 0;
	s16 y = 0;
	s16 last_load_at_x = -100;
	s16 last_load_at_y = -100;

	void update_layer(size_t);
	void load_map(size_t);
	ScreenEntry get_tile_from_camera(size_t, s16, s16);

	virtual void load_tilesets(size_t) = 0;
	virtual void load_palettes(size_t) = 0;
	virtual ScreenEntry get_tile(size_t, s16, s16) = 0;

  public:
	ScrollingMap(s16 width, s16 height)
		: Mode()
		, max_x(width)
		, max_y(height) {}

	ScrollingMap(
		s16 width, s16 height, size_t bg0_tile_source, size_t bg0_tile_map,
		size_t bg1_tile_source, size_t bg1_tile_map
	)
		: Mode()
		, bg0_tile_source(bg0_tile_source)
		, bg0_tile_map(bg0_tile_map)
		, bg1_tile_source(bg1_tile_source)
		, bg1_tile_map(bg1_tile_map)
		, max_x(width)
		, max_y(height) {}

	void update() override;
	void always_update() override;
	void restore() override;
	void suspend() override;
	void vsync_hook() override;
};

} // namespace scrolling_map
