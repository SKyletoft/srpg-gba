#include "cursor_scroller.h"

#include "hexes.h"
#include "input.h"
#include <array>
#include <tuple>

namespace cursor_scroller {

using hexes::Direction;
using input::Button;

void CursorScroller::update() {
	this->handle_input();
	this->update_layer(this->layer0);
	this->update_layer(this->layer1);
}


void CursorScroller::restore() {
	this->ScrollingMap::restore();

	std::memcpy(&tiles::SPRITE_PALETTE_MEMORY[0], arrowPal, sizeof(arrowPal));
	std::memcpy(tiles::SPRITE_CHARBLOCK[0], arrowTiles, sizeof(arrowTiles));

	REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;
}
void CursorScroller::handle_input() {
	std::array<std::tuple<Button, size_t, Direction>, 4> const inputs{
		std::tuple
		// Thanks C++ not needing nested {} for 2d arrays so this breaks instead
		{Button::Up, 0, Direction::UL},
		{Button::Down, 1, Direction::DL},
		{Button::Left, 2, Direction::L},
		{Button::Right, 3, Direction::R},
	};
	for (auto const &[button, index, dir] : inputs) {
		if (input::get_button(button).is_down()
			&& this->directional_cooldowns[index] >= 0)
		{
			this->cursor = this->cursor + dir;
			this->directional_cooldowns[index] = COOLDOWN;
		}
	}

	if (input::get_button(Button::R).is_down()
		&& input::get_button(Button::L).is_down())
	{
		state::next_state = 1;
	}
}

} // namespace cursor_scroller
