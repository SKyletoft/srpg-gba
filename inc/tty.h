#pragma once

#include "state.h"

namespace tty {

void decompress_1bpp_to_4bpp(void *dest, const u32 *__restrict src, size_t len);

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

	void clear();
};

} // namespace tty
