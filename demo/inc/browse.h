#pragma once

#include "map.h"

namespace browse {

class DefaultMap : public map::Map {
  public:
	void update() override;
};

} // namespace map
