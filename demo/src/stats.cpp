#include "stats.h"

#include "input.h"
#include "loading.h"
#include "state.h"
#include "tiles.h"
#include "tty.h"
#include "util.h"

#include <cstring>
#include <format>
#include <ranges>

extern "C" {
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

void Stats::restore() {
	util::wait_for_vsync();

	std::memcpy(CHARBLOCKS[TILE_SOURCE] + 1, fontTiles, fontTilesLen);
	tiles::BG_PALETTE_MEMORY[15] = loading::UI_PALETTE;

	for (size_t i : rv::iota(0uz, 1024uz)) {
		SCREENBLOCKS[TILE_MAP][i] = ScreenEntry(2, 0, 15);
	}

	std::string lines[]{
		std::string{this->data->name},
		"",
		std::format(
			"Health:  {:2}/{:2}",
			this->data->stats.health,
			this->data->stats.max_health
		),
		std::format("Attack:     {:2}", this->data->stats.attack),
		std::format("Defence:    {:2}", this->data->stats.defence),
		std::format("Magic:      {:2}", this->data->stats.magic),
		std::format("Resistance: {:2}", this->data->stats.resistance),
		std::format("Speed:      {:2}", this->data->stats.speed),
		std::format("Luck:       {:2}", this->data->stats.luck),
		std::format("Movement:   {:2}", this->data->stats.movement),
	};

	constexpr size_t Y_OFFSET = 4;
	constexpr size_t X_OFFSET = 13;

	for (auto [y, s] : lines | v::enumerate) {
		for (auto const [x, c] : s | v::enumerate | v::take(s.size())) {
			auto const y_ = (size_t)y + Y_OFFSET;
			auto const x_ = (size_t)x + X_OFFSET;
			SCREENBLOCKS[TILE_MAP][y_ * 32 + x_] =
				ScreenEntry(tty::get_character_tile_index(c + 2), 0, 15);
		}
	}

	REG_BG2HOFS = 0;
	REG_BG2VOFS = 0;
	REG_BG2CNT = BG_CBB(TILE_SOURCE) | BG_SBB(TILE_MAP) | BG_4BPP | BG_REG_32x32
				 | BG_PRIO(2);
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG2;
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
