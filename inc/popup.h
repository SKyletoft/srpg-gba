#pragma once

#include "sprite.h"
#include "state.h"
#include <functional>
#include <initializer_list>
#include <string>
#include <tuple>
#include <vector>

extern "C" {
#include <tonc_types.h>
}

namespace popup {

using sprite::Sprite;

class PopupMenu : public state::Mode {

	// TODO: Replace std::vector with something more platform appropriate
	std::vector<std::tuple<std::string, std::function<void()>>> entries;

	size_t const tile_source0;
	size_t const tile_source1;
	size_t const sprite_tile_source;
	size_t const tile_map0;
	size_t const tile_map1;

	s16 x = 0;
	s16 y = 0;
	s16 selection = 0;

	Sprite cursor {
		.y = 76,
		.x = 116,
		.tile_index = 0,
		.palette = 15,
	};

  public:
	bool blackout() override;

	void update() override;
	void always_update() override;
	void suspend() override;
	void restore() override;
	void vsync_hook() override;

	void set_position(s16, s16);

	PopupMenu();

	PopupMenu(
		std::initializer_list<std::tuple<std::string, std::function<void()>>> l
	)
		: Mode()
		, entries(l)
		, tile_source0(28)
		, tile_source1(27)
		, sprite_tile_source(4)
		, tile_map0(2)
		, tile_map1(3) {}
};

} // namespace popup
