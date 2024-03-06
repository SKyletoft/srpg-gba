#pragma once

#include "hexes.h"
#include "state.h"
#include "tiles.h"

#include <string_view>
#include <variant>

namespace dialogue {

using tiles::Palette;

constexpr u16 TILE_SOURCE_2 = 2;
constexpr u16 TILE_MAP_2 = 3;
constexpr u16 TILE_SOURCE_3 = 1;
constexpr u16 TILE_MAP_3 = 4;
constexpr size_t LEFT_PAL = 0;
constexpr size_t RIGHT_PAL = 3;

struct LoadCharacter {
	bool is_left;
	size_t portrait;
};

struct Text {
	std::string_view text;
	bool highlight_left;
};

struct FocusCursor {
	hexes::CubeCoord hex;
};

using TextBoxEvent = std::variant<Text, FocusCursor, LoadCharacter>;

constexpr std::array<TextBoxEvent, 10> TEXT{
	LoadCharacter{true, 1},
	LoadCharacter{false, 0},
	Text{
		"Oh woe is me!\nMy life is an infinite dark-ness and all happiness "
		"has\ncome to an end.",
		true
	},
	Text{"If only we weren't\nsurrounded by the enemy!", true},
	Text{"Look over there!\nThe leader!", false},
	FocusCursor{hexes::CubeCoord::from_offset_xy({5, 15})},
	LoadCharacter{true, 3},
	Text{"Gwaahahaha!", true},
	LoadCharacter{false, 1},
	Text{"Time to fight, I guess!", false},
};

class Dialogue : public state::Mode {
	Palette left_focus, left_defocus, right_focus, right_defocus;
	size_t step;
	std::span<const TextBoxEvent> items{TEXT};

  public:
	bool blackout() override;
	void restore() override;
	void update() override;

	void draw();
	void next();
};

} // namespace dialogue
