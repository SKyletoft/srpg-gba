#include "dialogue.h"

#include "audio.h"
#include "config.h"

#include "input.h"
#include "loading.h"
#include "map.h"
#include "state.h"
#include "stats.h"
#include "tiles.h"
#include "tty.h"

#include <ranges>

extern "C" {
#include <tonc.h>

#include "arrow.h"
}

namespace dialogue {

using tiles::CHARBLOCKS;
using tiles::SCREENBLOCKS;
using tiles::ScreenEntry;

namespace rv = std::ranges::views;
namespace v = std::views;

void load_text(std::string_view s) {
	constexpr size_t Y_OFFSET_TEXT = 0;
	constexpr size_t X_OFFSET_TEXT = 0;

	size_t y = 16;
	size_t x = 1;
	for (auto const c : s | v::take(s.size())) {
		if (x + X_OFFSET_TEXT > 28) {
			x = 1;
			y += 1;
		}
		if (c == '\n') {
			x = 1;
			y += 1;
			continue;
		}
		auto const y_ = y + Y_OFFSET_TEXT;
		auto const x_ = x + X_OFFSET_TEXT;
		SCREENBLOCKS[TILE_MAP_2][y_ * 32 + x_] =
			ScreenEntry(tty::get_character_tile_index(c + 2), 0, 15);

		x++;
	}
}

void clear_text() {
	for (size_t y : rv::iota(15uz, 20uz)) {
		for (size_t x : rv::iota(0uz, 30uz)) {
			SCREENBLOCKS[TILE_MAP_2][y * 32 + x] = ScreenEntry(2, 0, 15);
		}
	}
}

Palette darken(Palette pal) {
	for (auto &colour : pal.colours) {
		auto col = colour.rgb();
		col.red /= 2;
		col.green /= 2;
		col.blue /= 2;
		colour = col;
	}
	return pal;
}

bool Dialogue::blackout() { return false; }

void Dialogue::restore() {
	if (state::blacked_out) {
		loading::load_all();
	}

	for (size_t y : rv::iota(0uz, 15uz)) {
		for (size_t x : rv::iota(0uz, 30uz)) {
			SCREENBLOCKS[TILE_MAP_2][y * 32 + x] = ScreenEntry(0, 0, 0);
		}
	}
	clear_text();

	REG_BG2HOFS = 0;
	REG_BG2VOFS = 0;
	REG_BG3HOFS = 0;
	REG_BG3VOFS = 0;
	REG_BG2CNT = BG_CBB(TILE_SOURCE_2) | BG_SBB(TILE_MAP_2) | BG_4BPP
				 | BG_REG_32x32 | BG_PRIO(0);
	REG_BG3CNT = BG_CBB(TILE_SOURCE_3) | BG_SBB(TILE_MAP_3) | BG_4BPP
				 | BG_REG_32x32 | BG_PRIO(0);
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_BG3
				  | DCNT_OBJ | DCNT_OBJ_1D;

	if (this->step == this->items.size()) {
		this->step = 0;
	}
	this->draw();
}

void Dialogue::restart() { this->step = 0; }

void end() {
	state::next_state = 6;
	config::map.end_enemy_turn();
	config::overlay.image = overlay::Image::Rout;
}

void Dialogue::update() {
	if (input::get_button(input::Button::B) == input::InputState::Pressed) {
		end();
	}
	if (input::get_button(input::Button::A) == input::InputState::Pressed) {
		this->next();
	}
}

template <class... Ts> struct Overloaded : Ts... {
	using Ts::operator()...;
};
template <class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

void Dialogue::next() {
	this->step += 1;
	if (this->step == this->items.size()) {
		end();
		return;
	}
	this->draw();
}

void Dialogue::draw() {
	std::visit(
		Overloaded{
			[&](Text const &t) {
				audio::play_sfx(SFX__BLIP);
				clear_text();
				load_text(t.text);
				if (t.highlight_left) {
					tiles::BG_PALETTE_MEMORY[LEFT_PAL] = this->left_focus;
					tiles::BG_PALETTE_MEMORY[RIGHT_PAL] = this->right_defocus;
				} else {
					tiles::BG_PALETTE_MEMORY[LEFT_PAL] = this->left_defocus;
					tiles::BG_PALETTE_MEMORY[RIGHT_PAL] = this->right_focus;
				}
			},
			[&](LoadCharacter const &lc) {
				if (lc.is_left) {
					stats::load_portrait(lc.portrait, LEFT_PAL, 0, 0, false);
					this->left_focus = tiles::BG_PALETTE_MEMORY[LEFT_PAL];
					this->left_defocus = darken(this->left_focus);
				} else {
					stats::load_portrait(lc.portrait, RIGHT_PAL, 0, 14, true);
					this->right_focus = tiles::BG_PALETTE_MEMORY[RIGHT_PAL];
					this->right_defocus = darken(this->right_focus);
				}
				for (size_t y : rv::iota(15uz, 20uz)) {
					for (size_t x : rv::iota(0uz, 30uz)) {
						SCREENBLOCKS[TILE_MAP_3][y * 32 + x] =
							ScreenEntry(0, 0, 0);
					}
				}
				this->next();
			},
			[&](FocusCursor const &fc) {
				state::next_state = 0;
				config::map.state = map::MapState::AnimatingCutscene;
				config::cursor.cursor.move_to(fc.hex);
				this->next();
			},
		},
		this->items[this->step]
	);
}

} // namespace dialogue
