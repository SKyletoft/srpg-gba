#include "hexes.h"
#include <cstdlib>

namespace hexes {

OffsetXYCoord cube_to_offsetXY(CubeCoord hex) {
	auto col = hex.q + (hex.r - (hex.r & 1)) / 2;
	auto row = hex.r;
	return OffsetXYCoord((s16)col, (s16)row);
};

CubeCoord offsetXY_to_cube(OffsetXYCoord hex) {
	auto q = hex.col - (hex.row - (hex.row & 1)) / 2;
	auto r = hex.row;
	auto s = -q - r;
	return CubeCoord((s16)q, (s16)r, (s16)s);
};

CubeCoord axial_to_cube(AxialCoord hex) {
	s16 q = hex.q;
	s16 r = hex.r;
	s16 s = (s16)-q - (s16)r;
	return CubeCoord(q, r, s);
};

AxialCoord cube_to_axial(CubeCoord hex) {
	s16 q = hex.q;
	s16 r = hex.r;
	return AxialCoord(q, r);
};

OffsetXYCoord axial_to_offsetXY(AxialCoord hex) {
	CubeCoord cube = axial_to_cube(hex);
	return cube_to_offsetXY(cube);
};

AxialCoord offsetXY_to_axial(OffsetXYCoord hex) {
	CubeCoord cube = offsetXY_to_cube(hex);
	return cube_to_axial(cube);
};



s16 CubeCoord::distance(CubeCoord hex) const {
	CubeCoord vec = this->subtract(hex);
	auto dist = (std::abs(vec.q) + std::abs(vec.r) + std::abs(vec.s)) / 2;
	return (s16)dist;
};

} // namespace hexes