#include "input.h"

#include <array>

namespace input {

std::array<InputState, 10> BUTTON_STATES;

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

} // namespace input
