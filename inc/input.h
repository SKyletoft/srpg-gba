#pragma once

#include <array>
#include <cstddef>

namespace input {

class Button {
  public:
	enum InternalButton {
		A = 0,
		B,
		Select,
		Start,
		Right,
		Left,
		Up,
		Down,
		R,
		L,
	};

  private:
	InternalButton const button = A;

  public:
	bool operator==(InternalButton const &rhs) const;
	bool operator==(Button const &rhs) const;

	constexpr operator size_t() const { return (size_t)this->button; }

	Button(InternalButton const &from)
		: button(from) {}
	Button() {}
};

class InputState {
  public:
	enum InternalInputState { Up, Pressed, Held, Released };

  private:
	InternalInputState state = InputState::Up;

  public:
	bool is_down();
	bool is_up();

	bool operator==(InternalInputState const &rhs) const;
	bool operator==(InputState const &rhs) const;

	InputState(InternalInputState const &from)
		: state(from) {}
	InputState() {}
};

void poll();
std::array<InputState, 10> const &get_input();

} // namespace input
