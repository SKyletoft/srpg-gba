#pragma once

extern "C" {
#include <tonc_types.h>
}

namespace hexes {

struct CubeCoord {
  s16 s;
  s16 r;
  s16 q;
};

struct OffsetXYCoord {
  s16 col;
  s16 row;
};

OffsetXYCoord cube_to_offsetXY(CubeCoord hex);
CubeCoord OffsetXY_to_cube(OffsetXYCoord hex);

}
