#include "sprite.h"

#include <cstring>

namespace sprite {

void Sprite::write_to_screen(size_t hardware_sprite_id) {
	SPRITE_MEM[hardware_sprite_id] = *this;
}

} // namespace sprite
