#pragma once

#include "cursor_scroller.h"
#include "debug.h"
#include "export.h"
#include "hexmap.h"
#include "sprite.h"
#include "tiles.h"
#include "tty.h"
#include <array>

#include "context_menu.h"
#include "map.h"
#include "soundbank.h"
#include "test_map.h"
#include "unit.h"

namespace config {

using tiles::BG_PALETTE_MEMORY;
using tiles::SPRITE_PALETTE_MEMORY;
using unit::Stats;
using unit::Unit;

extern cursor_scroller::CursorScroller cursor;
extern hexmap::Hexmap hexmap;
extern std::array<Unit, 12> user_army;
extern size_t user_soldier_count;
extern map::Map map;
extern context_menu::ContextMenu popup;
extern std::array<state::Mode *, 7> const modes_data;
extern u32 startup_song;

} // namespace config
