#include <hexes.h>

namespace hexes {

OffsetXYCoord cube_to_offsetXY(CubeCoord hex) {
  auto col = hex.q + (hex.r - (hex.r & 1)) / 2;
  auto row = hex.r;
  return OffsetXYCoord((s16) col, (s16) row);
};

CubeCoord OffsetXY_to_cube(OffsetXYCoord hex){
  auto q = hex.col - (hex.row - (hex.row & 1)) / 2;
  auto r = hex.row;
  return CubeCoord((s16) q, (s16) r, (s16) -q-r);
};

}
