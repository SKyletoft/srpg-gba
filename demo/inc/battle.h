#pragma once

#include "state.h"

namespace battle {

class Battle : public state::Mode {
	u8 frame = 0;

  public:
	void update() override;
	void restore() override;
	void vsync_hook() override;
};

} // namespace battle
