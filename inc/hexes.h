#pragma once

extern "C" {
#include <tonc_types.h>
}

namespace hexes {

struct CubeCoord {
  s16 q;
  s16 r;
  s16 s;
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

}
