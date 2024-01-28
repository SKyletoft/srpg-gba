#pragma once
#include "tiles.h"

namespace hex_overlay_3_4 {

static constexpr tiles::STile hex00 = tiles::STile{
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
static constexpr tiles::STile hex01 = tiles::STile{
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
static constexpr tiles::STile hex02 = tiles::STile{
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

static constexpr tiles::STile hex10 = tiles::STile{
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

static constexpr tiles::STile hex20 = tiles::STile{
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
static constexpr tiles::STile hex21 = tiles::STile{
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
static constexpr tiles::STile hex22 = tiles::STile{
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

static constexpr tiles::STile hex31 = tiles::STile{
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

static constexpr tiles::STile empty = tiles::STile{
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
