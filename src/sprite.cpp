#include "sprite.h"

#include <bit>
#include <cstring>

namespace sprite {

void Sprite::write_to_screen(size_t hardware_sprite_id) {
	// SPRITE_MEM[hardware_sprite_id] = *this;
	((vu64 *)MEM_OAM)[hardware_sprite_id] = std::bit_cast<u64>(*this);
}

} // namespace sprite
