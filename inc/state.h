#pragma once

#include <cstddef>

extern "C" {
#include <tonc_types.h>
}

namespace state {

extern size_t next_state;

/// The mode baseclass is an abstraction to handle switching between game
/// states.
///
class Mode {
  private:
  public:
	const bool blackout;

	virtual void update() = 0;
	virtual void always_update() = 0;
	virtual void suspend() = 0;
	virtual void restore() = 0;
	virtual void vsync_hook() = 0;

	Mode()
		: blackout(true) {}
	Mode(bool blackout)
		: blackout(blackout) {}
	virtual ~Mode() {}
};

} // namespace state
