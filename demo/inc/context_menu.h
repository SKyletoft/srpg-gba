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
	void suspend() override;
};

} // namespace context_menu
