#pragma once

#include "tiles.h"

namespace numbers {

using STile = sprites::STile;

extern constexpr STile ONE =
	{.index_octs = {
		 0x00000000,
		 0x00010000,
		 0x00011000,
		 0x00010000,
		 0x00010000,
		 0x00010000,
		 0x01111100,
		 0x00000000,
	 }};

constexpr STile TWO =
	{.index_octs = {
		 0x00000000,
		 0x00111000,
		 0x01000100,
		 0x00100000,
		 0x00011000,
		 0x00000100,
		 0x01111110,
		 0x00000000,
	 }};

constexpr STile THREE =
	{.index_octs = {
		 0x00000000,
		 0x00111100,
		 0x01000010,
		 0x00110000,
		 0x01000000,
		 0x01000010,
		 0x00111100,
		 0x00000000,
	 }};
} // namespace numbers
