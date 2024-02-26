#include "hl-map.h"

#include "config.h"
#include "hexes.h"
#include "hexmap.h"
#include "tiles.h"

namespace hl_map {

using hexes::CubeCoord;

ScreenEntry HighlightMap::get_tile(Layer &layer, s16 x, s16 y) {
	ScreenEntry original_tile = hexmap::Hexmap::get_tile(layer, x, y);

	CubeCoord cc = this->grid_coord(layer, x, y).first.to_cube_coord();
	unit::Unit *unit = config::selected_unit;
	if (unit != nullptr && unit->pos().distance(cc) < unit->stats.movement) {
		original_tile.palette = 1;
	}

	return original_tile;
}

} // namespace hl_map
