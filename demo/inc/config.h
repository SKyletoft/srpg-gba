#pragma once

#include "cursor_scroller.h"
#include "tiles.h"

#include "browse.h"
#include "context_menu.h"
#include "hl-map.h"
#include "move_unit.h"
#include "soundbank.h"
#include "unit.h"
#include "battle.h"

#include "set.h"
#include <array>

namespace config {

using tiles::BG_PALETTE_MEMORY;
using tiles::SPRITE_PALETTE_MEMORY;
using unit::Stats;
using unit::Unit;

extern Unit *selected_unit;
extern Set<hexes::CubeCoord> highlights;
extern cursor_scroller::CursorScroller cursor;
extern hl_map::HighlightMap hexmap;
extern std::array<Unit, 12> user_army;
extern size_t user_soldier_count;

extern battle::Battle battle_ani;
extern browse::DefaultMap map;
extern move_unit::MoveUnit move;
extern context_menu::ContextMenu popup;
extern std::array<state::Mode *, 7> const modes_data;
extern u32 startup_song;

} // namespace config
