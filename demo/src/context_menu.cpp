#include "context_menu.h"
#include "sprite.h"
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

} // namespace context_menu
