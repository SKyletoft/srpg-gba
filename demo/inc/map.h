#pragma once

#include "cursor_scroller.h"
#include "hexmap.h"
#include "point.h"
#include "state.h"

namespace map {

using point::Point;

class Map : public state::Mode {
  public:
	cursor_scroller::CursorScroller cursor{};
	hexmap::Hexmap hexmap;

	template <size_t WIDTH, size_t HEIGHT>
	Map(std::array<std::array<u8, WIDTH>, HEIGHT> const &map)
		: cursor()
		, hexmap(map) {}

	void update() override;
	void always_update() override;
	void suspend() override;
	void restore() override;
	void vsync_hook() override;
	bool blackout() override;

	constexpr Point<s16> pos() const { return this->hexmap.layer0.pos; }
};

} // namespace map
