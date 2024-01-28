#pragma once
#include "tiles.h"

namespace hex_overlay_3_4 {

static constexpr sprites::STile hex00 = sprites::STile{
	.index_octs =
		{
			0x00000000,
			0x00000000,
			0x00000001,
			0x00000110,
			0x00011000,
			0x01100000,
			0x10000000,
			0x10000000,
		}
};
static constexpr sprites::STile hex01 = sprites::STile{
	.index_octs =
		{
			0x00011100,
			0x01100011,
			0x10000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
		}
};
static constexpr sprites::STile hex02 = sprites::STile{
	.index_octs =
		{
			0x00000000,
			0x00000000,
			0x11000000,
			0x00110000,
			0x00001100,
			0x00000011,
			0x00000000,
			0x00000000,
		}
};

static constexpr sprites::STile hex10 = sprites::STile{
	.index_octs =
		{
			0x10000000,
			0x10000000,
			0x10000000,
			0x10000000,
			0x10000000,
			0x10000000,
			0x10000000,
			0x10000000,
		}
};

static constexpr sprites::STile hex20 = sprites::STile{
	.index_octs =
		{
			0x11000000,
			0x00110000,
			0x00001100,
			0x00000011,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
		}
};
static constexpr sprites::STile hex21 = sprites::STile{
	.index_octs =
		{
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x11000001,
			0x00110110,
			0x00001000,
			0x00001000,
		}
};
static constexpr sprites::STile hex22 = sprites::STile{
	.index_octs =
		{
			0x00000001,
			0x00000110,
			0x00011000,
			0x01100000,
			0x10000000,
			0x00000000,
			0x00000000,
			0x00000000,
		}
};

static constexpr sprites::STile hex31 = sprites::STile{
	.index_octs =
		{
			0x00001000,
			0x00001000,
			0x00001000,
			0x00001000,
			0x00001000,
			0x00001000,
			0x00001000,
			0x00001000,
		}
};

static constexpr sprites::STile empty = sprites::STile{
	.index_octs =
		{
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
		}
};

} // namespace hex_overlay_3_4
