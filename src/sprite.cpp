#include "sprite.h"

#include <bit>

namespace sprite {

void HardwareSprite::write_to_screen(size_t hardware_sprite_id) {
	((vu64 *)MEM_OAM)[hardware_sprite_id] = std::bit_cast<u64>(*this);
}

} // namespace sprite
