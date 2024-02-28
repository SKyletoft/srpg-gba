#pragma once

#include "sprite.h"
#include "state.h"

namespace battle {

using sprite::HardwareSprite;

class Battle : public state::Mode {
	u8 frame = 0;
	u32 time = 0;

	HardwareSprite left{
		.object_mode = sprite::ObjectMode::Normal,
		.x = 20,
		.size = 0b011,
		.tile_index = 1,
		.palette = 0,
	};
	HardwareSprite right{
		.object_mode = sprite::ObjectMode::Normal,
		.x = 150,
		.horizontal_flip = true,
		.size = 0b011,
		.tile_index = 1,
		.palette = 1,
	};

  public:
	void update() override;
	void restore() override;
	void vsync_hook() override;
};

} // namespace battle
