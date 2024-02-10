#pragma once

#include "state.h"

extern "C" {
#include <tonc_types.h>
}

namespace tty {

void decompress_1bpp_to_4bpp(void *dest, const u32 *__restrict src, size_t len);

class TtyMode : public state::Mode {
  public:
	static constexpr size_t LINE_LEN = 30;
	static constexpr size_t COLUMN_LEN = 20;
	static constexpr size_t SIZE = LINE_LEN * COLUMN_LEN;

  private:
	char buffer[SIZE];
	size_t len = 0;
	bool in_focus = false;

	void draw_buffer();
	void draw_char(size_t i);
	void pad_to_newline();

  public:
	TtyMode()
		: Mode() {}

	void update() override;
	void always_update() override;
	void suspend() override;
	void restore() override;
	void vsync_hook() override;

	void clear();
	void clear_screen();
	void clear_buffer();
	void scroll_down();
	void print(const char *);
	void printf(const char *format, ...);
	void print(const char *, const size_t);
	void println(const char *);
	void println(const char *, const size_t);
};

} // namespace tty
