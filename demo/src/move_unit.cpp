#include "move_unit.h"

#include "input.h"
#include "state.h"

#include "config.h"

namespace move_unit {

using input::Button;
using input::InputState;

void MoveUnit::update() {
	this->animation_cycle = (u8)((this->animation_cycle + 1) % 1024);

	auto const d =
		config::cursor.move_cursor(config::hexmap.layer0.pos.into<s32>());
	config::hexmap.move_in_bounds(d.x, d.y);

	// Terminal on both shoulders
	if (input::get_button(Button::R).is_down()
		&& input::get_button(Button::L).is_down())
	{
		state::next_state = 1;
		config::selected_unit = nullptr;
	}

	if (input::get_button(Button::B) == InputState::Pressed) {
		state::next_state = 0;
		config::selected_unit = nullptr;
	}

	if (input::get_button(Button::A) == InputState::Pressed
		&& config::selected_unit->pos().distance(config::cursor.cursor.pos)
			   <= config::selected_unit->stats.movement)
	{
		state::next_state = 0;
		auto diff = config::selected_unit->pos() - config::cursor.cursor.pos;
		config::selected_unit->pos() = config::cursor.cursor.pos;
		config::selected_unit->sprite.animation =
			diff.to_pixel_space().into<s16>();
		config::selected_unit = nullptr;
	}
}

} // namespace move_unit
