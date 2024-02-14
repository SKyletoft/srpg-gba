#include "popup.h"

#include "debug.h"
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
using tiles::BG_PALETTE_MEMORY;
using tiles::CHARBLOCKS;
using tiles::Colour;
using tiles::Palette;
using tiles::SCREENBLOCKS;
using tiles::ScreenEntry;
using tiles::SPRITE_CHARBLOCK;
using tiles::SPRITE_PALETTE_MEMORY;

bool PopupMenu::blackout() { return false; }

void PopupMenu::update() {
	if (input::get_button(Button::B).is_down()) {
		state::next_state = 0;
	}

	if (input::get_button(Button::A) == input::InputState::Pressed) {
		auto &[_, f] = this->entries[this->selection];
		f();
	}

	if (input::get_button(Button::Up) == input::InputState::Pressed) {
		if (this->selection == 0) {
			this->selection = (u16)this->entries.size();
		}
		this->selection--;
	}

	if (input::get_button(Button::Down) == input::InputState::Pressed) {
		this->selection = (u16)((this->selection + 1) % this->entries.size());
	}
}

void PopupMenu::always_update() {}

void PopupMenu::suspend() {
	REG_DISPCNT &= ~(u32)(DCNT_BG3 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D);
}

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
	SPRITE_CHARBLOCK[0][1] = tiles::STile{{
		0x00033000,
		0x00033300,
		0x33333330,
		0x33333333,
		0x33333334,
		0x44433340,
		0x00033400,
		0x00044000,
	}};

	BG_PALETTE_MEMORY[15] = Palette{{
		tiles::TRANSPARENT,
		tiles::WHITE,
		// clangd does not consider this a constant expression, gcc does
		Colour::from_24bit_colour(198, 164, 89),
		tiles::RED,
		Colour(31, 15, 15),
	}};
	SPRITE_PALETTE_MEMORY[15] = BG_PALETTE_MEMORY[15];

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
		auto const y_ = (y + (size_t)this->y);
		auto const x_ = x + (size_t)this->x;
		SCREENBLOCKS[this->tile_map0][y_ * 32 + x_] = ScreenEntry(1, 0, 15);
	}
	for (auto const [y, t] : this->entries | v::enumerate) {
		auto [s, l] = t;
		for (auto const [x, c] : s | v::enumerate) {
			auto const y_ = y + this->y;
			auto const x_ = x + 34 + this->x;
			// Indent by one for the cursor to fit
			SCREENBLOCKS[this->tile_map1][y_ * 32 + x_] =
				ScreenEntry(tty::get_character_tile_index(c), 0, 15);
		}
	}

	REG_BG2HOFS = 0;
	REG_BG2VOFS = 0;
	REG_BG3HOFS = 0;
	REG_BG3VOFS = 0;

	util::wait_for_drawing_complete();

	this->cursor.write_to_screen(0);

	REG_BG2CNT = (u16)(BG_CBB(this->tile_source0) | BG_SBB(this->tile_map0)
					   | BG_4BPP | BG_REG_32x32 | BG_PRIO(1));
	REG_BG3CNT = (u16)(BG_CBB(this->tile_source1) | BG_SBB(this->tile_map1)
					   | BG_4BPP | BG_REG_32x32 | BG_PRIO(0));
	REG_DISPCNT |= DCNT_BG3 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D;
}

void PopupMenu::vsync_hook() {
	this->cursor.x = (u8)((this->x * 8) % 240) + 8;
	this->cursor.y = (u8)((this->y * 8) % 160 + 8 * (1 + this->selection));
	this->cursor.write_to_screen(0);
}

void PopupMenu::set_position(s16 x, s16 y) {
	this->x = x;
	this->y = y;
}

PopupMenu::PopupMenu()
	: Mode()
	, entries({
		  {"Red",
		   []() {
			   debug::println("Setting text to red");
			   BG_PALETTE_MEMORY[15].colours[1] = tiles::RED;
		   }},
		  {"Green",
		   []() {
			   debug::println("Setting text to green");
			   BG_PALETTE_MEMORY[15].colours[1] = tiles::GREEN;
		   }},
		  {"Blue",
		   []() {
			   debug::println("Setting text to blue");
			   BG_PALETTE_MEMORY[15].colours[1] = tiles::BLUE;
		   }},
		  {"Exit", []() { state::next_state = 0; }},
	  })
	, tile_source0(2)
	, tile_source1(3)
	, sprite_tile_source(4)
	, tile_map0(27)
	, tile_map1(28) {}

} // namespace popup
