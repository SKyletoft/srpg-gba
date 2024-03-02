#pragma once

#include "tiles.h"

namespace loading {

using tiles::Colour;

constexpr tiles::Palette BLUE_ACTIVE{
	tiles::TRANSPARENT,
	Colour::from_24bit_colour(0x1C, 0x71, 0xD8),
	Colour::from_24bit_colour(0x30, 0x30, 0x30),
	Colour::from_24bit_colour(0x00, 0xBD, 0xEA),
	Colour::from_24bit_colour(0x18, 0x34, 0x97),
};

constexpr tiles::Palette BLUE_USED{
	tiles::TRANSPARENT,
	Colour::from_24bit_colour(0x66, 0x66, 0xC8),
	Colour::from_24bit_colour(0x30, 0x30, 0x30),
	Colour::from_24bit_colour(0x7D, 0x7D, 0xDA),
	Colour::from_24bit_colour(0x3B, 0x3B, 0x87),
};

constexpr tiles::Palette RED_ACTIVE{
	tiles::TRANSPARENT,
	Colour::from_24bit_colour(0xC0, 0x1C, 0x28),
	Colour::from_24bit_colour(0x30, 0x30, 0x30),
	Colour(31, 0, 0),
	Colour(5, 0, 0),
};

constexpr tiles::Palette RED_USED{
	tiles::TRANSPARENT,
	Colour::from_24bit_colour(0x68, 0x1C, 0x28),
	Colour::from_24bit_colour(0x30, 0x30, 0x30),
	Colour(16, 0, 0),
	Colour(2, 0, 0),
};
constexpr tiles::Palette UI_PALETTE{
	tiles::TRANSPARENT,
	Colour::from_24bit_colour(198, 164, 89),
	tiles::WHITE,
	Colour(25, 25, 25),
};

void load_map_graphics();

}
