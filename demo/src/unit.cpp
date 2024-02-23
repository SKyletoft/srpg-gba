#include "unit.h"

namespace unit {

void Unit::render(Point<s16> camera_offset) {
	// TODO: Handle animation frames
	this->sprite.render(camera_offset);
}

} // namespace unit
