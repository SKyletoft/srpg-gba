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

using sprite::HardwareSprite;

class PopupMenu : public state::Mode {

	// TODO: Replace std::vector with something more platform appropriate
	std::vector<std::tuple<std::span<const char>, std::function<void()>>>
		entries;

	size_t const tile_source;
	size_t const sprite_tile_source;
	size_t const tile_map;

	s16 x = 10;
	s16 y = 6;
	u16 selection = 0;

	HardwareSprite cursor{
		.y = 76,
		.object_mode = sprite::ObjectMode::Normal,
		.x = 116,
		.horizontal_flip = true,
		.tile_index = 1,
		.palette = 15,
	};

  public:
	void update() override;
	void always_update() override;
	void suspend() override;
	void restore() override;
	void vsync_hook() override;

	void set_position(s16, s16);
	void load_tiles_and_palettes();

	PopupMenu();

	PopupMenu(
		std::initializer_list<std::tuple<char const *, std::function<void()>>> l
	)
		: Mode()
		, tile_source(27)
		, sprite_tile_source(4)
		, tile_map(3) {

		this->entries.reserve(l.size() - this->entries.capacity());
		for (auto &[s, f] : l) {
			this->entries.push_back({std::span{s, std::strlen(s)}, f});
		}
	}

	PopupMenu(
		std::initializer_list<std::tuple<char const *, std::function<void()>>>
			l,
		size_t tile_source, size_t sprite_tile_source, size_t tile_map
	)
		: Mode()
		, tile_source(tile_source)
		, sprite_tile_source(sprite_tile_source)
		, tile_map(tile_map) {}
};

} // namespace popup
