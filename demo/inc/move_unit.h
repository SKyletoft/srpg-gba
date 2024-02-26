#pragma once

#include "map.h"

namespace move_unit {

class MoveUnit : public map::Map {
  public:
	void update() override;
};

} // namespace move_unit
