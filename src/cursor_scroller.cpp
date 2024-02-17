#include "cursor_scroller.h"

#include "debug.h"
#include "hexes.h"
#include "input.h"
#include "tiles.h"
#include <array>
#include <cstring>
#include <tuple>

extern "C" {
#include "arrow.h"
}

namespace cursor_scroller {

using hexes::Direction;
using input::Button;

void CursorScroller::update() {
	auto old_cursor = this->cursor;
	auto old_offset = this->cursor_animation;
	this->handle_input();

	auto const xy = this->cursor.to_offset_xy();

	Point<s32> screen_centre{
		.x = layer0.x + 120,
		.y = layer0.y + 80,
	};

	Point<s32> const cursor{
		.x = xy.col * 24 + 12 * (xy.row & 1),
		.y = xy.row * 16,
	};

	Point<s16> d{};
	// 240px wide, split in two = 120px, with 30px buffer = 90px
	if (screen_centre.x - cursor.x < -90 + 16) {
		d.x = 1;
	} else if (screen_centre.x - cursor.x > 90) {
		d.x = -1;
	}
	// 160px tall, split in two = 80px, with a 30px buffer = 50px
	if (screen_centre.y - cursor.y < -50 + 16) {
		d.y = 1;
	} else if (screen_centre.y - cursor.y > 50) {
		d.y = -1;
	}
	this->move_in_bounds(d.x, d.y);

	if (screen_centre.x - cursor.x < -120 + 16
		|| screen_centre.x - cursor.x > 120
		|| screen_centre.y - cursor.y < -80 + 16
		|| screen_centre.y - cursor.y > 80)
	{
		this->cursor = old_cursor;
		this->cursor_animation = old_offset;
	} else {
		this->cursor_sprite.x =
			(u8)(cursor.x - this->layer0.x + 5 + this->cursor_animation.x);
		this->cursor_sprite.y =
			(u8)(cursor.y - this->layer0.y - 4 + this->cursor_animation.y);

		this->update_layer(this->layer0);
		this->update_layer(this->layer1);
	}

	this->cursor_animation.x =
		(s16)((this->cursor_animation.x * (s16)3) / (s16)4);
	this->cursor_animation.y =
		(s16)((this->cursor_animation.y * (s16)3) / (s16)4);
}

void CursorScroller::vsync_hook() {
	this->ScrollingMap::vsync_hook();
	this->cursor_sprite.write_to_screen(0);
}

void CursorScroller::restore() {
	this->ScrollingMap::restore();

	std::memcpy(&tiles::SPRITE_PALETTE_MEMORY[0], arrowPal, sizeof(arrowPal));
	std::memcpy(&tiles::SPRITE_CHARBLOCK[0][1], arrowTiles, sizeof(arrowTiles));

	REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;
}

void CursorScroller::handle_input() {
	bool const is_odd = this->cursor.is_odd();

	auto const [up_dir, up_offset] =
		is_odd ? std::tuple{Direction::UL, Point<s16>{12, 16}}
			   : std::tuple{Direction::UR, Point<s16>{-12, 16}};
	auto const [down_dir, down_offset] =
		is_odd ? std::tuple{Direction::DL, Point<s16>{12, -16}}
			   : std::tuple{Direction::DR, Point<s16>{-12, -16}};

	std::array<std::tuple<Button, size_t, Direction, Point<s16>>, 4> const
		inputs{
			std::tuple
			// Thanks C++ not needing nested {} for 2d arrays so
			// this breaks instead
			{Button::Up, 0, up_dir, up_offset},
			{Button::Down, 1, down_dir, down_offset},
			{Button::Left, 2, Direction::L, Point<s16>{24, 0}},
			{Button::Right, 3, Direction::R, Point<s16>{-24, 0}},
		};
	for (auto const &[button, index, dir, animation_offset] : inputs) {
		if (input::get_button(button).is_down()
			&& this->directional_cooldowns[index] <= 0)
		{
			this->cursor = this->cursor + dir;
			this->directional_cooldowns[index] = COOLDOWN;
			this->cursor_animation = this->cursor_animation + animation_offset;
		}
		this->directional_cooldowns[index]--;
	}

	if (input::get_button(Button::R).is_down()
		&& input::get_button(Button::L).is_down())
	{
		state::next_state = 2;
	}

	if (input::get_button(Button::A) == input::InputState::Pressed) {
		state::next_state = 3;
	}
}

} // namespace cursor_scroller
