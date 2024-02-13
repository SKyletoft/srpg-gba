#pragma once

#include <array>
#include <cstddef>
#include <optional>

extern "C" {
#include <tonc_types.h>
}

namespace hexes {

struct CubeCoord;
struct OffsetXYCoord;
struct AxialCoord;

enum class Direction { R, UR, UL, L, DL, DR };

struct CubeCoord {
	s16 const q;
	s16 const r;
	s16 const s;

  private:
	constexpr CubeCoord(s16 q, s16 r, s16 s)
		: q(q)
		, r(r)
		, s(s) {}

	constexpr static bool check_invariant(s16 q, s16 r, s16 s) {
		return (q + r + s) == 0;
	}

  public:

	s16 distance(CubeCoord) const;
	constexpr CubeCoord operator+(CubeCoord vec) const {
		return this->add(vec);
	}
	constexpr CubeCoord operator*(s16 factor) const {
		return this->scale(factor);
	}
	constexpr CubeCoord operator-(CubeCoord vec) const {
		return this->subtract(vec);
	}

	constexpr CubeCoord add(CubeCoord const vec) const {
		return CubeCoord(this->q + vec.q, this->r + vec.r, this->s + vec.s);
	}
	constexpr CubeCoord subtract(CubeCoord vec) const {
		return CubeCoord(this->q - vec.q, this->r - vec.r, this->s - vec.s);
	}
	constexpr CubeCoord scale(s16 factor) const {
		return CubeCoord(this->q * factor, this->r * factor, this->s * factor);
	}


	constexpr CubeCoord neighbour(Direction dir) const;

	static constexpr std::optional<CubeCoord> from_qrs(s16 q, s16 r, s16 s) {
		if (check_invariant(q, r, s)) {
			return CubeCoord(q, r, s);
		}
		return std::nullopt;
	}

	static constexpr CubeCoord unsafe_from_qrs_unchecked(s16 q, s16 r, s16 s) {
		return CubeCoord(q, r, s);
	}

	friend CubeCoord offsetXY_to_cube(OffsetXYCoord);
	friend CubeCoord axial_to_cube(AxialCoord);
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

static constexpr std::array<CubeCoord, 6> CUBE_DIRECTION_VECTORS = {
	CubeCoord::unsafe_from_qrs_unchecked(1, 0, -1),
	CubeCoord::unsafe_from_qrs_unchecked(1, -1, 0),
	CubeCoord::unsafe_from_qrs_unchecked(0, -1, 1),
	CubeCoord::unsafe_from_qrs_unchecked(-1, 0, 1),
	CubeCoord::unsafe_from_qrs_unchecked(-1, 1, 0),
	CubeCoord::unsafe_from_qrs_unchecked(0, 1, -1),
};

constexpr CubeCoord CubeCoord::neighbour(Direction dir) const {
	return CUBE_DIRECTION_VECTORS[(size_t)dir];
};

} // namespace hexes
