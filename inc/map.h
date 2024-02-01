#pragma once

#include "state.h"
#include "tiles.h"

extern "C" {
#include <tonc_types.h>
}

namespace map {

using TileRep = tiles::TileRep;
using STile = tiles::STile;
using ObjectAttribute = OBJ_ATTR;
using ObjectAffineAttributes = OBJ_AFFINE;

static constexpr int BG0_TILE_SOURCE = 1;
static constexpr int BG1_TILE_SOURCE = 0;
static constexpr int BG0_TILE_MAP = 29;
static constexpr int BG1_TILE_MAP = 30;
static constexpr int SPRITE_SOURCE = 4;

class MapMode : public state::Mode {
  private:
	ObjectAttribute obj_buffer[128];
	ObjectAffineAttributes *obj_aff_buffer;

	ObjectAttribute *arrow = &obj_buffer[0];
	int x = 2;
	int y = 2;
	int t = 0;
	int cooldown = 0;
	static constexpr int COOLDOWN_TIMER = 8;

	void load_metr();
	void load_fe8();
	void load_hexgrid();
	void handle_input();

  public:
	MapMode()
		: Mode() {}

	void update() override;
	void always_update() override;
	void restore() override;
	void vsync_hook() override;
};

} // namespace map
