#include "overlay.h"

#include "config.h"
#include "loading.h"

#include "point.h"
#include "state.h"
#include "tiles.h"
#include "util.h"

#include <algorithm>
#include <cstring>
#include <ranges>
#include <tonc_memmap.h>

extern "C" {
#include "enemy-turn-overlay.h"
#include "player-turn-overlay.h"
#include "rout-the-enemy-overlay.h"

#include <tonc.h>
}

namespace overlay {

namespace rv = std::ranges::views;

using point::Point;
using tiles::BG_PALETTE_MEMORY;
using tiles::CHARBLOCKS;
using tiles::Palette;
using tiles::SCREENBLOCKS;
using tiles::ScreenEntry;

size_t coord(Point<u32> point) { return (size_t)(point.y * 32 + point.x); }

void draw_logo(u32 from) {
	u32 x = from % 20;
	u32 y = from / 20;

	size_t const tile_idx = coord({x, y});
	u16 const se_idx = (u16)(y * 20 + x) + 1;
	SCREENBLOCKS[TILE_MAP][tile_idx] = ScreenEntry(se_idx, 0, 15);
}

bool Overlay::blackout() { return false; }

void Overlay::update() {
	if (this->time < 220 / 5) {
		for (u32 i = this->time * 5; i < (this->time + 1) * 5; ++i) {
			draw_logo(i);
		}
	}
	if (this->time >= 90) {
		state::next_state = 0;
	}
	this->time++;
}

void Overlay::restore() {
	this->time = 0;

	REG_DISPCNT &= (u16) ~(DCNT_BG3 | DCNT_BG2);

	if (this->image == Image::Enemy) {
		BG_PALETTE_MEMORY[15] = *(Palette *)enemy_turn_overlayPal;
		std::memcpy(
			(void *)&CHARBLOCKS[TILE_SOURCE][1],
			enemy_turn_overlayTiles,
			sizeof(tiles::STile) * 11 * 20
		);
	} else if (this->image == Image::Player) {
		BG_PALETTE_MEMORY[15] = *(Palette *)player_turn_overlayPal;
		std::memcpy(
			(void *)&CHARBLOCKS[TILE_SOURCE][1],
			player_turn_overlayTiles,
			sizeof(tiles::STile) * 11 * 20
		);
	} else if (this->image == Image::Rout) {
		BG_PALETTE_MEMORY[15] = *(Palette *)rout_the_enemy_overlayPal;
		std::memcpy(
			(void *)&CHARBLOCKS[TILE_SOURCE][1],
			rout_the_enemy_overlayTiles,
			sizeof(tiles::STile) * 11 * 20
		);
	}

	for (auto [x, y] :
		 rv::cartesian_product(rv::iota(0u, 32u), rv::iota(0u, 32u)))
	{
		size_t const idx = coord({x, y});
		SCREENBLOCKS[TILE_MAP][idx] = ScreenEntry(0, 0, 15);
	}

	REG_BG3HOFS = (u16)-34;
	REG_BG3VOFS = (u16)-32;

	REG_BG3CNT = (u16)(BG_CBB(TILE_SOURCE) | BG_SBB(TILE_MAP) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(0));

	REG_DISPCNT |= (u16)(DCNT_BG3);
}

} // namespace overlay
