#include "stats.h"

#include "input.h"
#include "loading.h"
#include "state.h"
#include "tiles.h"
#include "tty.h"
#include "util.h"

#include <cstddef>
#include <cstring>
#include <format>
#include <ranges>
#include <tonc_memdef.h>

extern "C" {
#include "big-portrait-1.h"
#include "big-portrait-2.h"
#include "big-portrait-3.h"
#include "big-portrait-4.h"
#include "font.h"
}

namespace stats {

namespace rv = std::ranges::views;
namespace v = std::views;

using input::Button;
using input::InputState;
using tiles::CHARBLOCKS;
using tiles::SCREENBLOCKS;
using tiles::ScreenEntry;

bool Stats::blackout() { return false; }

constexpr std::array<std::tuple<void *, size_t, void *, void *>, 4> PORTRAITS{
	std::tuple<void *, size_t, void *, void *>{
		(void *)big_portrait_1Tiles,
		(size_t)big_portrait_1TilesLen,
		(void *)big_portrait_1Map,
		(void *)big_portrait_1Pal
	},
	std::tuple<void *, size_t, void *, void *>{
		(void *)big_portrait_2Tiles,
		(size_t)big_portrait_2TilesLen,
		(void *)big_portrait_2Map,
		(void *)big_portrait_2Pal
	},
	std::tuple<void *, size_t, void *, void *>{
		(void *)big_portrait_3Tiles,
		(size_t)big_portrait_3TilesLen,
		(void *)big_portrait_3Map,
		(void *)big_portrait_3Pal
	},
	std::tuple<void *, size_t, void *, void *>{
		(void *)big_portrait_4Tiles,
		(size_t)big_portrait_4TilesLen,
		(void *)big_portrait_4Map,
		(void *)big_portrait_4Pal
	},
};

void load_portrait(
	size_t idx, u8 palette, size_t y_offset, size_t x_offset, bool second
) {
	auto [tiles, tiles_len, map, pal] = PORTRAITS[idx];

	std::memcpy(CHARBLOCKS[TILE_SOURCE_3] + second * 256, tiles, tiles_len);
	tiles::BG_PALETTE_MEMORY[(size_t)palette] = *(tiles::Palette *)pal;
	for (size_t y = 0; y < 128 / 8; ++y) {
		for (size_t x = 0; x < 112 / 8; ++x) {
			size_t const offset_idx = (y + y_offset) * 32 + x + x_offset;
			size_t const idx = y * 32 + x;
			auto se = ((ScreenEntry *)map)[idx].with_palette(palette);
			se.index += (256 * second) & 0b1111111111;

			SCREENBLOCKS[TILE_MAP_3][offset_idx] = se;
		}
	}
}

void Stats::restore() {
	util::wait_for_vsync();

	std::memcpy(CHARBLOCKS[TILE_SOURCE_2] + 1, fontTiles, fontTilesLen);
	tiles::BG_PALETTE_MEMORY[15] = loading::UI_PALETTE;

	for (ScreenEntry volatile &se : SCREENBLOCKS[TILE_MAP_3]) {
		se = ScreenEntry(0, 0, 0);
	}
	for (size_t y : rv::iota(1uz, 19uz)) {
		for (size_t x : rv::iota(1uz, 29uz)) {
			SCREENBLOCKS[TILE_MAP_2][y * 32 + x] = ScreenEntry(2, 0, 15);
		}
	}
	for (size_t y : rv::iota(0uz, 20uz)) {
		SCREENBLOCKS[TILE_MAP_2][y * 32 + 0] = ScreenEntry(0, 0, 15);
		SCREENBLOCKS[TILE_MAP_2][y * 32 + 29] = ScreenEntry(0, 0, 15);
	}
	for (size_t x : rv::iota(1uz, 29uz)) {
		SCREENBLOCKS[TILE_MAP_2][0 * 32 + x] = ScreenEntry(0, 0, 15);
		SCREENBLOCKS[TILE_MAP_2][19 * 32 + x] = ScreenEntry(0, 0, 15);
	}

	load_portrait(this->data->portrait, 0, 1, 0, false);

	std::string lines[]{
		std::string{this->data->name},
		"",
		std::format(
			"Health: {:2}/{:2}",
			this->data->stats.health,
			this->data->stats.max_health
		),
		std::format("Attack:    {:2}", this->data->stats.attack),
		std::format("Defence:   {:2}", this->data->stats.defence),
		std::format("Magic:     {:2}", this->data->stats.magic),
		std::format("Resistance:{:2}", this->data->stats.resistance),
		std::format("Speed:     {:2}", this->data->stats.speed),
		std::format("Luck:      {:2}", this->data->stats.luck),
		std::format("Movement:  {:2}", this->data->stats.movement),
	};

	constexpr size_t Y_OFFSET_TEXT = 5;
	constexpr size_t X_OFFSET_TEXT = 15;

	for (auto [y, s] : lines | v::enumerate) {
		for (auto const [x, c] : s | v::enumerate | v::take(s.size())) {
			auto const y_ = (size_t)y + Y_OFFSET_TEXT;
			auto const x_ = (size_t)x + X_OFFSET_TEXT;
			SCREENBLOCKS[TILE_MAP_2][y_ * 32 + x_] =
				ScreenEntry(tty::get_character_tile_index(c + 2), 0, 15);
		}
	}

	REG_BG2HOFS = 0;
	REG_BG2VOFS = 0;
	REG_BG3HOFS = 0;
	REG_BG3VOFS = 0;
	REG_BG2CNT = BG_CBB(TILE_SOURCE_2) | BG_SBB(TILE_MAP_2) | BG_4BPP
				 | BG_REG_32x32 | BG_PRIO(1);
	REG_BG3CNT = BG_CBB(TILE_SOURCE_3) | BG_SBB(TILE_MAP_3) | BG_4BPP
				 | BG_REG_32x32 | BG_PRIO(0);
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_BG3;
}

void Stats::update() {
	if (input::get_button(Button::R) == InputState::Pressed
		|| input::get_button(Button::B) == InputState::Pressed)
	{
		state::next_state = 0;
	}
}

void Stats::set_unit(unit::Unit const &unit) { this->data = &unit; }

} // namespace stats
