#pragma once

#include "state.h"

namespace tty {

class TtyMode : public state::Mode {
  private:
  public:
	TtyMode()
		: Mode() {}

	void update() override;
	void always_update() override;
	void suspend() override;
	void restore() override;
	void vsync_hook() override;
};

} // namespace tty
