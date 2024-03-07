#pragma once

#include "popup.h"
#include <initializer_list>

namespace context_menu {

class ContextMenu : public popup::PopupMenu {
  public:
	ContextMenu()
		: PopupMenu() {}

	ContextMenu(
		std::initializer_list<std::pair<char const *, std::function<void()>>> l
	)
		: PopupMenu(l) {}

	bool blackout() override;
	void restore() override;
	void suspend() override;
	void b_hook() override;
	void move_hook() override;
	void selection_hook() override;
};

} // namespace context_menu
