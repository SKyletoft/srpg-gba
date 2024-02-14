#include "scrolling_map.h"

#include "input.h"
#include "state.h"
#include "tiles.h"
#include "util.h"
#include <algorithm>
#include <cstring>

extern "C" {
#include <tonc.h>
#include <tonc_memmap.h>
}

#define memcpy_(dest, src) std::memcpy((void *)dest, src, sizeof(src))

namespace scrolling_map {

using input::Button;

size_t get_screenblock_offset_from_tiles(s16 x, s16 y) {
	while (x < 0) {
		x += 32;
	}
	while (y < 0) {
		y += 32;
	}
	return ((((size_t)y) % 32) * 32) + (((size_t)x) % 32);
}

size_t get_screenblock_offset_from_camera(s16 x, s16 y) {
	return get_screenblock_offset_from_tiles(x / 8, y / 8);
}

ScreenEntry ScrollingMap::get_tile_from_camera(Layer &layer, s16 x, s16 y) {
	return this->get_tile(layer, x / 8, y / 8);
}

void ScrollingMap::update_tile(
	ScreenEntry volatile *const base, Layer &layer, s16 x, s16 y
) {
	size_t const idx = get_screenblock_offset_from_camera(x, y);
	ScreenEntry const tile = this->get_tile_from_camera(layer, x, y);
	base[idx] = tile;
};

void ScrollingMap::load_map(Layer &layer) {
	ScreenEntry volatile *const base = tiles::SCREENBLOCKS[layer.tile_map];

	// Yes, load one row too many
	for (s16 x = 0; x <= 30 * 8; x += 8) {
		for (s16 y = 0; y <= 20 * 8; y += 8) {
			this->update_tile(base, layer, layer.x + x, layer.y + y);
		}
	}
}

void ScrollingMap::update_layer(Layer &layer) {
	layer.x = std::clamp(
		(s16)(layer.x + input::horizontal_direction()), layer.min_x, layer.max_x
	);
	layer.y = std::clamp(
		(s16)(layer.y + input::vertical_direction()), layer.min_y, layer.max_y
	);

	ScreenEntry volatile *const base = tiles::SCREENBLOCKS[layer.tile_map];

	s16 const dy = layer.y - layer.last_load_at_y;
	if (abs(dy) > 4) {
		s16 const diff = dy < 0 ? -4 : 164;
		s16 const cam_y = layer.y + diff;

		for (s16 d_cam_x = -8; d_cam_x < 248; d_cam_x += 7) {
			this->update_tile(base, layer, layer.x + d_cam_x, cam_y);
		}

		layer.updated_y = true;
	}

	s16 const dx = layer.x - layer.last_load_at_x;
	if (abs(dx) > 4) {
		s16 const diff = dx < 0 ? -4 : 244;
		s16 const cam_x = layer.x + diff;
		for (s16 d_cam_y = -8; d_cam_y < 168; d_cam_y += 7) {
			this->update_tile(base, layer, cam_x, layer.y + d_cam_y);
		}
		layer.updated_x = true;
	}

	if (layer.updated_x) {
		layer.updated_x = false;
		layer.last_load_at_x = layer.x;
	}
	if (layer.updated_y) {
		layer.updated_y = false;
		layer.last_load_at_y = layer.y;
	}
}

void ScrollingMap::update() {
	this->update_layer(this->layer0);
	this->update_layer(this->layer1);

	if (input::get_button(Button::L).is_down()
		&& input::get_button(Button::R).is_down())
	{
		state::next_state = 2;
	}

	if (input::get_button(Button::A).is_down()) {
		state::next_state = 3;
	}
}

void ScrollingMap::always_update() {}

void ScrollingMap::restore() {
	this->load_tilesets(this->layer0);
	this->load_tilesets(this->layer1);
	this->load_map(this->layer0);
	this->load_map(this->layer1);
	util::wait_for_drawing_start();
	this->load_palettes(this->layer0);

	tiles::BG_PALETTE_MEMORY[15] = tiles::Palette{{
		tiles::TRANSPARENT,
		tiles::WHITE,
		// clangd does not consider this a constant expression, gcc does
		tiles::Colour::from_24bit_colour(198, 164, 89),
	}};

	REG_BG0CNT = (u16)(BG_CBB((u16)this->layer0.tile_source)
					   | BG_SBB((u16)this->layer0.tile_map) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(3));
	REG_BG1CNT = (u16)(BG_CBB((u16)this->layer1.tile_source)
					   | BG_SBB((u16)this->layer1.tile_map) | BG_4BPP
					   | BG_REG_32x32 | BG_PRIO(3));
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;

	this->vsync_hook();
}

void ScrollingMap::suspend() {}

void ScrollingMap::vsync_hook() {
	REG_BG0HOFS = (u16)this->layer0.x;
	REG_BG0VOFS = (u16)this->layer0.y;
	REG_BG1HOFS = (u16)this->layer1.x;
	REG_BG1VOFS = (u16)this->layer1.y;
}

bool ScrollingMap::blackout() {
	return !(
		(state::current_state == 0 && state::next_state == 3)
		|| (state::current_state == 3 && state::next_state == 0)
	);
}

} // namespace scrolling_map
