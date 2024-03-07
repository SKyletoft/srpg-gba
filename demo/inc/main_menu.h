#pragma once

#include "popup.h"

#include <initializer_list>

namespace main_menu {

class MainMenu : public popup::PopupMenu {
  public:
	MainMenu()
		: PopupMenu() {}

	MainMenu(
		std::initializer_list<std::pair<char const *, std::function<void()>>> l
	)
		: PopupMenu(l, 1, 4, 3) {}

	bool blackout() override;
	void restore() override;
	void load_tiles_and_palettes() override;
	void move_hook() override;
	void selection_hook() override;
};

} // namespace main_menu
