#include "browse.h"

#include "input.h"
#include "state.h"

#include "config.h"
#include "unit.h"

#include <algorithm>

namespace browse {

namespace r = std::ranges;

using input::Button;
using input::InputState;
using unit::Unit;

void DefaultMap::update() {
	this->Map::update();

	// Terminal on both shoulders
	if (input::get_button(Button::R).is_down()
		&& input::get_button(Button::L).is_down())
	{
		state::next_state = 1;
	}

	if (input::get_button(Button::A) == InputState::Pressed) {
		std::span<Unit> my_units{
			config::user_army.data(), config::user_soldier_count
		};
		auto selected_unit = r::find_if(my_units, [&](auto &unit) {
			return unit.sprite.pos == config::cursor.cursor.pos;
		});

		config::selected_unit = nullptr;
		if (selected_unit != my_units.end()) {
			config::selected_unit = &*selected_unit;
			state::next_state = 3;
		} else {
			state::next_state = 2;
		}
	}
}

} // namespace map
