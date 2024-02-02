#pragma once

#include "state.h"

namespace tty {

void decompress_1bpp_to_4bpp(void *dest, const u32 *__restrict src, size_t len);

class TtyMode : public state::Mode {
  private:
	static constexpr size_t LINE_LEN = 240 / 8;
	static constexpr size_t SIZE = 240 * 160 / (8 * 8);
	char buffer[SIZE];
	size_t len = 0;

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
