#pragma once

#include "sprite.h"
#include "state.h"
#include <algorithm>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>

extern "C" {
#include <tonc_types.h>
}

namespace popup {

namespace r = std::ranges;

using sprite::Sprite;

class PopupMenu : public state::Mode {

	// TODO: Replace std::vector with something more platform appropriate
	std::vector<std::tuple<std::span<const char>, std::function<void()>>>
		entries;

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
		std::initializer_list<std::tuple<char const *, std::function<void()>>> l
	)
		: Mode()
		, tile_source0(28)
		, tile_source1(27)
		, sprite_tile_source(4)
		, tile_map0(2)
		, tile_map1(3) {

		for (auto &[s, f] : l) {
			this->entries.push_back({std::span{s, std::strlen(s)}, f});
		}
	}

	PopupMenu(
		std::initializer_list<std::tuple<char const *, std::function<void()>>>
			l,
		size_t tile_source0, size_t tile_source1, size_t sprite_tile_source,
		size_t tile_map0, size_t tile_map1
	)
		: Mode()
		, tile_source0(tile_source0)
		, tile_source1(tile_source1)
		, sprite_tile_source(sprite_tile_source)
		, tile_map0(tile_map0)
		, tile_map1(tile_map1) {}
};

} // namespace popup
