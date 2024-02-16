#pragma once

#include "cursor_scroller.h"
#include "mdspan.h"
#include "tiles.h"

namespace hexmap {

using scrolling_map::Layer;
using tiles::Palette;
using tiles::ScreenEntry;
using tiles::STile;

static constexpr s16 WIDTH = (s16)40;
static constexpr s16 HEIGHT = (s16)29;

class Hexmap : public cursor_scroller::CursorScroller {
  protected:
	void load_tilesets(Layer &) override;
	void load_palettes(Layer &) override;
	ScreenEntry get_tile(Layer &, s16, s16) override;

	// std::array<std::array<u8, WIDTH>, HEIGHT> const &map;
	mdspan::Span2d<const u8> const map;

  public:
	Hexmap(std::array<std::array<u8, WIDTH>, HEIGHT> const &map)
		: CursorScroller(WIDTH * 3 * 8 - 240, HEIGHT * 16 - 155, 1, 26, 0, 30)
		, map(map) {}
};

class SimpleHexmap : public Hexmap {
  private:
	std::span<const STile> tiles;
	std::span<const Palette> palettes;

  protected:
	void load_tilesets(Layer &) override;
	void load_palettes(Layer &) override;

  public:
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
