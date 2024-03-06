#pragma once

#include "hexes.h"
#include "state.h"
#include "tiles.h"

#include <string_view>
#include <variant>

namespace dialogue {

class Dialogue : public state::Mode {
  public:
	bool blackout() override;
	void restore() override;
	void update() override;
};

} // namespace dialogue
