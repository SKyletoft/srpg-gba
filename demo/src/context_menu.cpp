#include "context_menu.h"

namespace context_menu {

bool ContextMenu::blackout() { return false; }

void ContextMenu::suspend() { this->PopupMenu::suspend(); }

} // namespace context_menu
