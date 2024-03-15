#include "main_menu.h"

#include "audio.h"
#include "sprite.h"
#include "state.h"
#include "tiles.h"
#include "tty.h"
#include "config.h"

#include "soundbank.h"

#include <ranges>

namespace main_menu {

namespace rv = std::ranges::views;

using tiles::CHARBLOCKS;

bool MainMenu::blackout() { return state::last_state == 3; }

void MainMenu::restore() {
	for (auto const idx : rv::iota(0uz, 128uz)) {
		sprite::HardwareSprite::hide(idx);
	}
	this->y = 10;
	this->PopupMenu::restore();
	audio::play_song(config::menu_song);
}

void MainMenu::load_tiles_and_palettes() {
	CHARBLOCKS[this->tile_source][0] = tiles::EMPTY;
	CHARBLOCKS[this->tile_source][97] = tiles::EMPTY;
	tty::decompress_1bpp_to_4bpp(
		&CHARBLOCKS[this->tile_source][2], sys8Glyphs, '~' - ' '
	);
	tiles::BG_PALETTE_MEMORY[15] = tiles::Palette{
		tiles::TRANSPARENT,
		tiles::YELLOW,
	};
}

void MainMenu::move_hook() { audio::play_sfx(SFX__BLIP); }

void MainMenu::selection_hook() { audio::play_sfx(SFX__BLIP); }

} // namespace main_menu
