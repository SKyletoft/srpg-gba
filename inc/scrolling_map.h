#pragma once

#include "state.h"
#include "tiles.h"

extern "C" {
#include <tonc_types.h>
}

namespace scrolling_map {

using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;

static constexpr int BG0_TILE_SOURCE = 0;
static constexpr int BG0_TILE_MAP = 30;

size_t get_screenblock_offset_from_tiles(s16 x, s16 y);
size_t get_screenblock_offset_from_camera(s16 x, s16 y);

class ScrollingMap : public state::Mode {
  private:
	const s16 max_x = 40 * 3 * 8;
	const s16 max_y = 30 * 4 * 8;
	s16 x = 0;
	s16 y = 0;
	s16 last_load_at_x = -100;
	s16 last_load_at_y = -100;

	void load_map();
	ScreenEntry get_tile_from_camera(s16, s16);

	virtual void load_tilesets() = 0;
	virtual void load_palettes() = 0;
	virtual ScreenEntry get_tile(s16, s16) = 0;

  public:
	ScrollingMap(s16 width, s16 height)
		: Mode()
		, max_x(width)
		, max_y(height) {}

	void update() override;
	void always_update() override;
	void restore() override;
	void suspend() override;
	void vsync_hook() override;
};

} // namespace scrolling_map
