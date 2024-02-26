#include "cursor_scroller.h"

#include "hexes.h"
#include "input.h"
#include <array>
#include <cstring>
#include <tuple>

namespace cursor_scroller {

using hexes::Direction;
using input::Button;

CubeCoord &CursorScroller::pos() { return this->cursor.pos; }

Point<s16> CursorScroller::move_cursor(Point<s32> const camera_position) {
	auto old_cursor = this->cursor;
	auto old_offset = this->cursor.animation;
	this->handle_input();

	Point<s32> const screen_centre = camera_position + Point{120, 80};
	Point<s32> const cursor = this->cursor.pos.to_pixel_space();

	Point<s16> d{};
	// 240px wide, split in two = 120px, with 30px buffer = 90px
	if (screen_centre.x - cursor.x < -90 + 16) {
		d.x = this->scroll_speed;
	} else if (screen_centre.x - cursor.x > 90) {
		d.x = -this->scroll_speed;
	}
	// 160px tall, split in two = 80px, with a 30px buffer = 50px
	if (screen_centre.y - cursor.y < -50 + 16) {
		d.y = this->scroll_speed;
	} else if (screen_centre.y - cursor.y > 50) {
		d.y = -this->scroll_speed;
	}

	this->cursor.animation.x =
		(s16)((this->cursor.animation.x * (s16)3) / (s16)4);
	this->cursor.animation.y =
		(s16)((this->cursor.animation.y * (s16)3) / (s16)4);

	if (screen_centre.x - cursor.x < -120 + 16
		|| screen_centre.x - cursor.x > 120
		|| screen_centre.y - cursor.y < -80 + 16
		|| screen_centre.y - cursor.y > 80)
	{
		this->cursor = old_cursor;
		this->cursor.animation = old_offset;
		return Point<s16>{0, 0};
	} else {
		return d;
	}
}

void CursorScroller::handle_input() {
	bool const is_odd = this->cursor.pos.is_odd();

	auto const up_dir = is_odd ? Direction::UL : Direction::UR;
	auto const down_dir = is_odd ? Direction::DL : Direction::DR;

	std::array<std::tuple<Button, size_t, Direction>, 4> const inputs{
		std::tuple
		// Thanks C++ not needing nested {} for 2d arrays so
		// this breaks instead
		{Button::Up, 0, up_dir},
		{Button::Down, 1, down_dir},
		{Button::Left, 2, Direction::L},
		{Button::Right, 3, Direction::R},
	};
	for (auto const &[button, index, dir] : inputs) {
		if (input::get_button(button).is_down()
			&& this->directional_cooldowns[index] <= 0)
		{
			auto const old_cur_screen = this->cursor.pos.to_pixel_space();
			this->cursor.pos += dir;
			auto const new_cur_screen = this->cursor.pos.to_pixel_space();
			this->directional_cooldowns[index] = this->cooldown;
			this->cursor.animation +=
				(old_cur_screen - new_cur_screen).into<s16>();
		}
		if (this->directional_cooldowns[index] > this->cooldown) {
			this->directional_cooldowns[index] = 0;
		}
		this->directional_cooldowns[index]--;
	}
}

} // namespace cursor_scroller
