#include "image.h"
#include "state.h"
#include "tiles.h"

#include <cstring>

extern "C" {
#include "game-over.h"
#include "title-screen.h"
#include "win.h"

#include <tonc.h>
}

namespace image {

BackgroundData backgrounds[] = {
	BackgroundData{
		game_overPal,
		game_overTiles,
		game_overMap,
		game_overPalLen,
		game_overTilesLen,
		game_overMapLen
	},
	BackgroundData{
		title_screenPal,
		title_screenTiles,
		title_screenMap,
		title_screenPalLen,
		title_screenTilesLen,
		title_screenMapLen
	},
	BackgroundData{winPal, winTiles, winMap, winPalLen, winTilesLen, winMapLen},
};

bool Image::blackout() {
	return !(
		state::current_state == 3
		&& (state::next_state == 7 || state::next_state == 8
			|| state::next_state == 9)
	);
}

void Image::update() {}

void Image::restore() {
	auto [pal, tiles, map, pal_len, tiles_len, map_len] =
		backgrounds[(size_t)this->bg];
	std::memcpy(tiles::BG_PALETTE_MEMORY.data(), pal, (size_t)pal_len);
	std::memcpy(tiles::CHARBLOCKS[TILE_SOURCE], tiles, (size_t)tiles_len);
	std::memcpy((void *)tiles::SCREENBLOCKS[TILE_MAP], map, (size_t)map_len);

	REG_BG0CNT = BG_CBB(TILE_SOURCE) | BG_SBB(TILE_MAP) | BG_4BPP | BG_REG_64x32
				 | BG_PRIO(3);
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

	state::next_state = 7 + (size_t)this->bg;
}

} // namespace image
