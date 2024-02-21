#pragma once

#include "popup.h"
#include <initializer_list>

namespace context_menu {

class ContextMenu : public popup::PopupMenu {
  public:
	ContextMenu()
		: PopupMenu() {}

	ContextMenu(
		std::initializer_list<std::tuple<char const *, std::function<void()>>> l
	)
		: PopupMenu(l) {}

	bool blackout() override;
};

} // namespace context_menu
