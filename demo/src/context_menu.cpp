#include "context_menu.h"

#include "audio.h"
#include "sprite.h"

#include "soundbank.h"

#include <ranges>

namespace context_menu {

namespace rv = std::ranges::views;

bool ContextMenu::blackout() { return false; }

void ContextMenu::restore() {
	for (auto const idx : rv::iota(0uz, 128uz)) {
		sprite::HardwareSprite::hide(idx);
	}
	this->PopupMenu::restore();
}

void ContextMenu::suspend() { this->PopupMenu::suspend(); }

void ContextMenu::b_hook() { state::next_state = 0; }

void ContextMenu::move_hook() { audio::play_sfx(SFX__BLIP); }

void ContextMenu::selection_hook() { audio::play_sfx(SFX__BLIP); }

} // namespace context_menu
