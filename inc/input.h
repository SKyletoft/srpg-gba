#pragma once

namespace input {

enum class Button { A, B, L, R, Start, Select, Up, Down, Left, Right };

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

} // namespace input
