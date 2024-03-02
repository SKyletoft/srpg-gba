#pragma once

#include "state.h"

namespace overlay {

constexpr size_t TILE_SOURCE = 2;
constexpr size_t TILE_MAP = 3;

class Overlay : public state::Mode {
  public:
	bool is_enemy = false;
	u32 time = 0;

	bool blackout() override;
	void update() override;
	void restore() override;

	Overlay()
		: Mode() {}
};

} // namespace overlay
