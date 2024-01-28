#include "hex-overlay.h"
#include "tiles.h"
#include "util.h"
#include <cstring>

extern "C" {
#include <tonc.h>
#include <tonc_memmap.h>
#include <tonc_types.h>

#include "fe8.h"
}

using namespace sprites;

constexpr int BG0_TILE_SOURCE = 1;
constexpr int BG1_TILE_SOURCE = 0;
constexpr int BG0_TILE_MAP = 29;
constexpr int BG1_TILE_MAP = 30;

void load_fe8() {
	// Load image from grit generated headers
	std::memcpy(pal_bg_mem, fe8Pal, fe8PalLen);
	std::memcpy(&tile_mem[BG0_TILE_SOURCE][0], fe8Tiles, fe8TilesLen);
	std::memcpy(&se_mem[BG0_TILE_MAP][0], fe8Map, fe8MapLen);
}

void load_hexgrid() {
	// Load hex grid from hand written tiles
	STile *tiles = (STile *)(tile_mem[BG1_TILE_SOURCE]);
	tiles[6] = hex_overlay_3_4::empty;
	tiles[7] = hex_overlay_3_4::hex00;
	tiles[8] = hex_overlay_3_4::hex01;
	tiles[9] = hex_overlay_3_4::hex02;
	tiles[10] = hex_overlay_3_4::hex10;
	tiles[11] = hex_overlay_3_4::hex20;
	tiles[12] = hex_overlay_3_4::hex21;
	tiles[13] = hex_overlay_3_4::hex22;
	tiles[14] = hex_overlay_3_4::hex31;

	auto const transparent = TileRep(6, 0, 0);

	auto const hex00 = TileRep(7, 0, 0);
	auto const hex01 = TileRep(8, 0, 0);
	auto const hex02 = TileRep(9, 0, 0);
	auto const hex10 = TileRep(10, 0, 0);
	auto const hex11 = transparent;
	auto const hex12 = transparent;
	auto const hex20 = TileRep(11, 0, 0);
	auto const hex21 = TileRep(12, 0, 0);
	auto const hex22 = TileRep(13, 0, 0);
	auto const hex30 = transparent;
	auto const hex31 = TileRep(14, 0, 0);
	auto const hex32 = transparent;

	for (size_t j = 0; j < 32; j += 4) {
		auto base0 = &se_mem[BG1_TILE_MAP][j * 32];
		for (size_t i = 0; i < 30; i += 3) {
			base0[i] = hex02;
			base0[i + 1] = hex01;
			base0[i + 2] = hex00;
		}
		base0[30] = hex02;
		base0[31] = hex01;

		auto base1 = &se_mem[BG1_TILE_MAP][(j + 1) * 32];
		for (size_t i = 0; i < 30; i += 3) {
			base1[i] = hex12;
			base1[i + 1] = hex11;
			base1[i + 2] = hex10;
		}
		base1[30] = hex12;
		base1[31] = hex11;

		auto base2 = &se_mem[BG1_TILE_MAP][(j + 2) * 32];
		for (size_t i = 0; i < 30; i += 3) {
			base2[i] = hex22;
			base2[i + 1] = hex21;
			base2[i + 2] = hex20;
		}
		base2[30] = hex22;
		base2[31] = hex21;

		auto base3 = &se_mem[BG1_TILE_MAP][(j + 3) * 32];
		for (size_t i = 0; i < 30; i += 3) {
			base3[i] = hex32;
			base3[i + 1] = hex31;
			base3[i + 2] = hex30;
		}
		base3[30] = hex32;
		base3[31] = hex31;
	}
}

int main() {
	load_fe8();
	load_hexgrid();

	REG_BG0CNT = BG_CBB(BG0_TILE_SOURCE) | BG_SBB(BG0_TILE_MAP) | BG_8BPP
				 | BG_REG_64x64 | BG_PRIO(1);
	REG_BG1CNT = BG_CBB(BG1_TILE_SOURCE) | BG_SBB(BG1_TILE_MAP) | BG_4BPP
				 | BG_REG_32x32 | BG_PRIO(0);
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;

	util::spin();

	return 0;
}
