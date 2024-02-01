#include "tty.h"

extern "C" {
#include <tonc.h>
}

namespace tty {

void TtyMode::update() {}

void TtyMode::always_update() {}

void TtyMode::suspend() {}

constexpr tiles::Palette YELLOW_ON_BLACK = tiles::Palette{{
	tiles::BLACK,
	tiles::YELLOW,
}};

/// len is counting compressed bytes
void decompress_1bpp_to_4bpp(
	void *dest, const u32 *__restrict src, size_t len
) {
	STile *dest_tile = (STile *)dest;

	for (size_t i = 0; i < len * 4; i += 2) {
		u32 bottom = src[i];
		u32 top = src[i + 1];

		STile tile{};
		for (size_t j = 0; j < 32; j += 2) {
			u32 low_nibble_mask = 1 << j;
			u32 high_nibble_mask = 1 << (j + 1);

			tile.raw[j / 2] = ((bottom & low_nibble_mask) != 0)
							  | (byte)(((bottom & high_nibble_mask) != 0) << 4);

			tile.raw[j / 2 + 16] =
				((top & low_nibble_mask) != 0)
				| (byte)(((top & high_nibble_mask) != 0) << 4);
		}

		*dest_tile = tile;
		dest_tile++;
	}
}

void TtyMode::restore() {
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
	tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));
}

void TtyMode::vsync_hook() {}

} // namespace tty
