#pragma once

#include "state.h"

#include <tuple>

namespace image {

constexpr u16 TILE_SOURCE = 2;
constexpr u16 TILE_MAP = 0;

using BackgroundData =
	std::tuple<u16 const *, u32 const *, u16 const *, s32, s32, s32>;

enum class Background {
	GameOver = 0,
	TitleScreen = 1,
	Win = 2,
};

class Image : public state::Mode {
  public:
	Background bg = Background::TitleScreen;

	bool blackout() override;
	void update() override;
	void restore() override;

	Image()
		: Mode() {}
};

} // namespace image
