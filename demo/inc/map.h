#pragma once

#include "cursor_scroller.h"
#include "hexmap.h"
#include "state.h"

namespace map {

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
};

} // namespace map
