#pragma once

#include "hexes.h"
#include "state.h"
#include "tiles.h"

#include <string_view>
#include <variant>

namespace dialogue {

constexpr u16 TILE_SOURCE_2 = 2;
constexpr u16 TILE_MAP_2 = 3;
constexpr u16 TILE_SOURCE_3 = 1;
constexpr u16 TILE_MAP_3 = 4;
constexpr size_t LEFT_PAL = 0;
constexpr size_t RIGHT_PAL = 3;
class Dialogue : public state::Mode {
  public:
	bool blackout() override;
	void restore() override;
	void update() override;
};

} // namespace dialogue
