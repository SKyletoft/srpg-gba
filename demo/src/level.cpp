#include "level.h"
#include "config.h"

namespace level {

void load_level(Level &level) {
	config::hexmap = hl_map::HighlightMap{level.map()};
	config::enemy_soldier_count = level.enemies().size();
	for (size_t i = 0; i < level.enemies().size(); ++i) {
		config::enemy_army[i] = level.enemies()[i];
	}
	config::user_army = config::default_user_army;
}

}; // namespace level
