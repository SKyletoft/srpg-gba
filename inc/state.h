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
	Mode() {}
	virtual ~Mode() {}

	virtual void update() = 0;
	virtual void restore() = 0;
	virtual void vsync_hook() = 0;
};

} // namespace state
