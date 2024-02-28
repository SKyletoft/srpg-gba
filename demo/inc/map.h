#pragma once

#include "point.h"
#include "state.h"

namespace map {

using point::Point;

class Map : public state::Mode {
  public:
	u8 animation_cycle = 0;

	void update() override;
	void restore() override;
	void vsync_hook() override;
	bool blackout() override;
};

} // namespace map
