#include "popup.h"

#include "input.h"
#include "state.h"
#include "tiles.h"
#include "tty.h"
#include "util.h"
#include <cstring>
#include <ranges>

extern "C" {
#include <tonc.h>
#include <tonc_memdef.h>

extern const u32 sys8Glyphs[192];
}

namespace popup {

namespace v = std::views;

using input::Button;
using tiles::CHARBLOCKS;
using tiles::Colour;
using tiles::Palette;
using tiles::PALETTE_MEMORY;
using tiles::SCREENBLOCKS;
using tiles::ScreenEntry;

bool PopupMenu::blackout() {
	return !(
		(state::current_state == 0 && state::next_state == 3)
		|| (state::current_state == 3 && state::next_state == 0)
	);
}

void PopupMenu::update() {
	if (input::get_button(Button::B).is_down()) {
		state::next_state = 0;
	}
}

void PopupMenu::always_update() {}
void PopupMenu::suspend() { REG_DISPCNT &= ~(u32)(DCNT_BG3 | DCNT_BG2); }

void PopupMenu::restore() {
	this->selection = 0;

	// We don't blackout, but we do disable gui
	util::wait_for_drawing_complete();
	REG_DISPCNT &= ~(u32)(DCNT_BG3 | DCNT_BG2);

	tty::decompress_1bpp_to_4bpp(
		CHARBLOCKS[this->tile_source1], sys8Glyphs, '~' - ' '
	);
	CHARBLOCKS[this->tile_source0][0] = tiles::EMPTY;
	CHARBLOCKS[this->tile_source0][1] = tiles::STile{{
		0x22222222,
		0x22222222,
		0x22222222,
		0x22222222,
		0x22222222,
		0x22222222,
		0x22222222,
		0x22222222,
	}};
	CHARBLOCKS[this->sprite_tile_source][0] = tiles::STile{{
		0x00011000,
		0x00011100,
		0x11111110,
		0x11111111,
		0x11111112,
		0x22211120,
		0x00011200,
		0x00022000,
	}};

	PALETTE_MEMORY[15] = Palette{{
		tiles::TRANSPARENT,
		tiles::WHITE,
		// clangd does not consider this a constant expression, gcc, does
		Colour::from_24bit_colour(198, 164, 89),
	}};

	util::clear_layer(this->tile_map0);
	util::clear_layer(this->tile_map1);

	size_t const menu_width = ([&]() {
		size_t longest = 0;
		for (auto const &[s, _] : this->entries) {
			if (s.size() > longest) {
				longest = s.size();
			}
		}
		return longest;
	})();
	size_t const menu_height = this->entries.size();

	for (auto const [y, x] : v::cartesian_product(
			 v::iota(0uz, menu_height + 2), v::iota(0uz, menu_width + 3)
		 ))
	{
		SCREENBLOCKS[this->tile_map0][y * 32 + x] = ScreenEntry(1, 0, 15);
	}
	for (auto const [y, t] : this->entries | v::enumerate) {
		auto [s, l] = t;
		for (auto const [x, c] : s | v::enumerate) {
			// Indent by one for the cursor to fit
			SCREENBLOCKS[this->tile_map1][y * 32 + x + 1] =
				ScreenEntry(tty::get_character_tile_index(c), 0, 15);
		}
	}

	REG_BG2HOFS = (u16)this->x;
	REG_BG2VOFS = (u16)this->y;
	REG_BG3HOFS = (u16)this->x - 8;
	REG_BG3VOFS = (u16)this->y - 8;

	util::wait_for_drawing_complete();
	REG_BG2CNT = (u16)(BG_CBB(this->tile_source0) | BG_SBB(this->tile_map0)
					   | BG_4BPP | BG_REG_32x32 | BG_PRIO(1));
	REG_BG3CNT = (u16)(BG_CBB(this->tile_source1) | BG_SBB(this->tile_map1)
					   | BG_4BPP | BG_REG_32x32 | BG_PRIO(0));
	REG_DISPCNT |= DCNT_BG3 | DCNT_BG2;
}

void PopupMenu::vsync_hook() {

}

PopupMenu::PopupMenu()
	: Mode()
	, entries({
		  {"Red", []() { PALETTE_MEMORY[15].colours[1] = tiles::RED; }},
		  {"Green", []() { PALETTE_MEMORY[15].colours[1] = tiles::GREEN; }},
		  {"Blue", []() { PALETTE_MEMORY[15].colours[1] = tiles::BLUE; }},
		  {"Exit", []() { state::next_state = 0; }},
	  })
	, tile_source0(2)
	, tile_source1(3)
	, sprite_tile_source(4)
	, tile_map0(27)
	, tile_map1(28) {}

} // namespace popup
