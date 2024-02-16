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
	this->handle_input();
	this->update_layer(this->layer0);
	this->update_layer(this->layer1);
}

void CursorScroller::vsync_hook() {
	this->ScrollingMap::vsync_hook();

	auto const xy = this->cursor.to_offset_xy();
	this->cursor_sprite.x =
		(u8)(xy.col * 24 + 12 * (xy.row & 1) + this->layer0.x + 5);
	this->cursor_sprite.y = (u8)(xy.row * 16 + this->layer0.y - 4);
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
	std::array<std::tuple<Button, size_t, Direction>, 4> const inputs{
		std::tuple
		// Thanks C++ not needing nested {} for 2d arrays so this breaks instead
		{Button::Up, 0, is_odd ? Direction::UL : Direction::UR},
		{Button::Down, 1, is_odd ? Direction::DL : Direction::DR},
		{Button::Left, 2, Direction::L},
		{Button::Right, 3, Direction::R},
	};
	for (auto const &[button, index, dir] : inputs) {
		if (input::get_button(button).is_down()
			&& this->directional_cooldowns[index] <= 0)
		{
			this->cursor = this->cursor + dir;
			this->directional_cooldowns[index] = COOLDOWN;

			debug::clear();
			debug::println(this->cursor.q);
			debug::println(this->cursor.r);
			debug::println(this->cursor.s);
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
