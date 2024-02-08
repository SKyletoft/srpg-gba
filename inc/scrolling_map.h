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

struct Layer {
	const size_t tile_source;
	const size_t tile_map;

	const s16 min_x = 0;
	const s16 min_y = 0;
	const s16 max_x = 40 * 3 * 8;
	const s16 max_y = 30 * 4 * 8;

	s16 x = 0;
	s16 y = 0;
	s16 last_load_at_x = -100;
	s16 last_load_at_y = -100;
	bool updated_x = false;
	bool updated_y = false;

	bool operator==(Layer const &) const = default;
	bool operator!=(Layer const &) const = default;
};

class ScrollingMap : public state::Mode {
  protected:
	Layer layer0;
	Layer layer1;

	void update_layer(Layer &);
	void load_map(Layer &);
	ScreenEntry get_tile_from_camera(Layer &, s16, s16);

	virtual void load_tilesets(Layer &) = 0;
	virtual void load_palettes(Layer &) = 0;
	virtual ScreenEntry get_tile(Layer const &, s16, s16) = 0;

  public:
	ScrollingMap(s16 width, s16 height)
		: Mode()
		, layer0(Layer{
			  .tile_source = 0,
			  .tile_map = 30,
			  .max_x = width,
			  .max_y = height,
			  .x = 0
		  })
		, layer1(Layer{
			  .tile_source = 1,
			  .tile_map = 29,
			  .min_x = 4,
			  .max_x = (s16)(width + 4),
			  .max_y = height,
			  .x = 4
		  }) {}

	ScrollingMap(
		s16 width, s16 height, size_t bg0_tile_source, size_t bg0_tile_map,
		size_t bg1_tile_source, size_t bg1_tile_map
	)
		: Mode()
		, layer0(Layer{
			  .tile_source = bg0_tile_source,
			  .tile_map = bg0_tile_map,
			  .max_x = width,
			  .max_y = height,
			  .x = 0
		  })
		, layer1(Layer{
			  .tile_source = bg1_tile_source,
			  .tile_map = bg1_tile_map,
			  .min_x = 4,
			  .max_x = (s16)(width + 4),
			  .max_y = height,
			  .x = 4
		  }) {}

	void update() override;
	void always_update() override;
	void restore() override;
	void suspend() override;
	void vsync_hook() override;
};

} // namespace scrolling_map
