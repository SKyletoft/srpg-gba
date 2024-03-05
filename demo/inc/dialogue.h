#pragma once

#include "state.h"

namespace dialogue {

class Dialogue : public state::Mode {
  public:
	bool blackout() override;
	void restore() override;
	void update() override;
};

} // namespace dialogue
