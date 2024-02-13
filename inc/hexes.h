#pragma once

extern "C" {
#include <tonc_types.h>
}

namespace hexes {

enum class Direction { R, UR, UL, L, DL, DR };

struct CubeCoord {
	s16 q;
	s16 r;
	s16 s;

	CubeCoord add(CubeCoord) const;
	CubeCoord operator+(CubeCoord) const;
	CubeCoord subtract(CubeCoord) const;
	CubeCoord operator-(CubeCoord) const;
	CubeCoord scale(s16) const;
	CubeCoord operator*(s16) const;

	CubeCoord neighbour(Direction) const;
	s16 distance(CubeCoord) const;
};

struct OffsetXYCoord {
	s16 col;
	s16 row;
};

struct AxialCoord {
	s16 q;
	s16 r;
};

OffsetXYCoord cube_to_offsetXY(CubeCoord hex);
OffsetXYCoord axial_to_offsetXY(AxialCoord hex);

CubeCoord offsetXY_to_cube(OffsetXYCoord hex);
CubeCoord axial_to_cube(AxialCoord hex);

AxialCoord cube_to_axial(CubeCoord hex);
AxialCoord offsetXY_to_axial(OffsetXYCoord hex);

CubeCoord const CUBE_DIRECTION_VECTORS[] = {
	CubeCoord(1, 0, -1),
	CubeCoord(1, -1, 0),
	CubeCoord(0, -1, 1),
	CubeCoord(-1, 0, 1),
	CubeCoord(-1, 1, 0),
	CubeCoord(0, 1, -1),
};

} // namespace hexes
