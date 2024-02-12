#include "input.h"

#include <array>

extern "C" {
#include <tonc_memmap.h>
}

namespace input {

bool InputState::is_down() {
	return this->state == InputState::Pressed
		   || this->state == InputState::Held;
}

bool InputState::is_up() { return !this->is_down(); }

bool InputState::operator==(InternalInputState const &rhs) const {
	return this->state == rhs;
}

bool InputState::operator==(InputState const &rhs) const {
	return this->state == rhs.state;
}

std::array<InputState, 10> BUTTON_STATES;

void poll() {
	Button const buttons[] = {
		Button::A,
		Button::B,
		Button::L,
		Button::R,
		Button::Start,
		Button::Select,
		Button::Up,
		Button::Down,
		Button::Left,
		Button::Right
	};

	u16 raw = REG_KEYINPUT;

	for (auto button : buttons) {
		bool down = (bool)(raw & (1 << button));
		if (down) {
			BUTTON_STATES[button] = BUTTON_STATES[button].is_down()
										? InputState::Held
										: InputState::Pressed;
		} else {
			BUTTON_STATES[button] = BUTTON_STATES[button].is_down()
										? InputState::Released
										: InputState::Up;
		}
	}
}

std::array<InputState, 10> const &get_input() { return BUTTON_STATES; }

} // namespace input
