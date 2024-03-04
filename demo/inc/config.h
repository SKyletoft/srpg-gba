#pragma once

#include "cursor_scroller.h"
#include "tiles.h"

#include "battle.h"
#include "context_menu.h"
#include "hl-map.h"
#include "map.h"
#include "overlay.h"
#include "soundbank.h"
#include "unit.h"

#include "set.h"
#include <array>
#include <span>

namespace config {

using hexes::CubeCoord;
using sprite::HexSprite;
using tiles::BG_PALETTE_MEMORY;
using tiles::SPRITE_PALETTE_MEMORY;
using unit::Stats;
using unit::Unit;

extern Unit *selected_unit;
extern Set<hexes::CubeCoord> highlights;
extern CubeCoord original_pos;
extern std::vector<Unit *> neighbouring_enemies;
extern Set<Unit *> used;
extern cursor_scroller::CursorScroller cursor;
extern hl_map::HighlightMap hexmap;
extern std::array<Unit, 8> user_army;
extern size_t user_soldier_count;

extern std::array<Unit, 20> enemy_army;
extern size_t enemy_soldier_count;

extern overlay::Overlay overlay;
extern battle::Battle battle_ani;
extern map::Map map;
extern context_menu::ContextMenu popup;
extern context_menu::ContextMenu movement_popup;

std::span<Unit> user_units();
std::span<Unit> enemy_units();

} // namespace config
