#include "sprite.h"

#include <bit>

namespace sprite {

void HardwareSprite::write_to_screen(size_t hardware_sprite_id) {
	((vu64 *)MEM_OAM)[hardware_sprite_id] = std::bit_cast<u64>(*this);
}

void HardwareSprite::hide(size_t hardware_sprite_id) {
	HardwareSprite{.object_mode = ObjectMode::Hidden}.write_to_screen((size_t
	)hardware_sprite_id);
}

void HexSprite::hide() const {
	HardwareSprite::hide((size_t)this->hardware_id);
}

void HexSprite::render(Point<s16> const camera_offset) const {
	auto const pixel_space = this->pos.to_pixel_space().into<s16>();
	auto const screen_space = pixel_space - camera_offset;

	s32 x = screen_space.x + this->animation.x + this->centre.x;
	s32 y = screen_space.y + this->animation.y + this->centre.y;

	if (x < -16 || x > 240 || y < -16 || y > 160) {
		HardwareSprite::hide(this->hardware_id);
		return;
	}

	HardwareSprite{
		.y = (u8)y,
		.object_mode = ObjectMode::Normal,
		.colour_mode = this->colour_mode,
		.shape = (u8)(((u8)this->size & 0b1100) >> 2),
		.x = (u16)(x & 0b111'111'111),
		.horizontal_flip = this->horizontal_flip,
		.vertical_flip = this->vertical_flip,
		.size = (u8)((u8)this->size & 0b11),
		.tile_index = this->tile_index,
		.prio = this->prio,
		.palette = this->palette,
	}
		.write_to_screen(this->hardware_id);
}

} // namespace sprite
