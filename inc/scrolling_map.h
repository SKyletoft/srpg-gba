#pragma once

#include "state.h"
#include "tiles.h"

extern "C" {
#include <tonc_types.h>
}

namespace scrolling_map {

using ScreenEntry = tiles::ScreenEntry;
using STile = tiles::STile;

static constexpr int BG0_TILE_SOURCE = 1;
static constexpr int BG1_TILE_SOURCE = 0;
static constexpr int BG0_TILE_MAP = 29;
static constexpr int BG1_TILE_MAP = 30;
static constexpr int SPRITE_SOURCE = 4;

class ScrollingMap : public state::Mode {
  private:
	int x = 0;
	int y = 0;

  public:
	ScrollingMap()
		: Mode() {}

	void update() override;
	void always_update() override;
	void restore() override;
	void suspend() override;
	void vsync_hook() override;
};

} // namespace scrolling_map
