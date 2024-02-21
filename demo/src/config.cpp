#include "export.h"
#include "tty.h"
#include <array>

#include "context_menu.h"
#include "map.h"
#include "soundbank.h"
#include "test_map.h"

namespace debug {
extern tty::TtyMode tty_mode;
}

namespace config {

map::Map map{test_map::map};
context_menu::ContextMenu popup{};

std::array<state::Mode *, 3> const modes_data{
	&map,
	&debug::tty_mode,
	&popup,
};

u32 startup_song = MOD_BAD_APPLE;

EXPORT(modes_data, startup_song);

} // namespace config
