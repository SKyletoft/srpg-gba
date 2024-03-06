#pragma once

#include "state.h"
#include "unit.h"

namespace stats {

constexpr u16 TILE_SOURCE_2 = 2;
constexpr u16 TILE_MAP_2 = 3;
constexpr u16 TILE_SOURCE_3 = 1;
constexpr u16 TILE_MAP_3 = 4;

void load_portrait(size_t, u8, size_t, size_t, bool);

class Stats : public state::Mode {
	unit::Unit const *data = nullptr;

  public:
	bool blackout() override;
	void restore() override;
	void update() override;

	void set_unit(unit::Unit const &);
};

} // namespace stats
