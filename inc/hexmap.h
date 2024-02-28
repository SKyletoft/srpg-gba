#pragma once

#include "hexes.h"
#include "mdspan.h"
#include "point.h"
#include "scrolling_map.h"
#include "tiles.h"

namespace hexmap {

using hexes::OffsetXYCoord;
using point::Point;
using scrolling_map::Layer;
using tiles::Palette;
using tiles::ScreenEntry;
using tiles::STile;

static constexpr s16 WIDTH = (s16)40;
static constexpr s16 HEIGHT = (s16)29;

class Hexmap : public scrolling_map::ScrollingMap {
  public:
	mdspan::Span2d<const u8> const map;

	std::pair<hexes::OffsetXYCoord, Point<s16>>
	grid_coord(Layer &layer, s16 x, s16 y);

	void load_tilesets(Layer &) override;
	void load_palettes(Layer &) override;
	ScreenEntry get_tile(Layer &, s16, s16) override;

	Hexmap(std::array<std::array<u8, WIDTH>, HEIGHT> const &map)
		: ScrollingMap(WIDTH * 3 * 8 - 240, HEIGHT * 16 - 155, 1, 26, 0, 30)
		, map(map) {}
};

class SimpleHexmap : public Hexmap {
  private:
	std::span<const STile> tiles;
	std::span<const Palette> palettes;

  public:
	void load_tilesets(Layer &) override;
	void load_palettes(Layer &) override;

	SimpleHexmap(
		std::array<std::array<u8, WIDTH>, HEIGHT> const &map,
		std::span<const STile> const tiles,
		std::span<const Palette> const palettes
	)
		: Hexmap(map)
		, tiles(tiles)
		, palettes(palettes) {}
};

} // namespace hexmap
