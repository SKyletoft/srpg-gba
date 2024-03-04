#pragma once

#include "mdspan.h"
#include "unit.h"

#include <span>

extern "C" {
#include <tonc_types.h>
}

namespace level {

class Level {
  public:
	constexpr Level() {}

	constexpr virtual mdspan::Span2d<const u8> map() = 0;
	constexpr virtual std::span<const unit::Unit> enemies() = 0;

	constexpr virtual ~Level() {}
};

#define DECLARE_LEVEL(name, map_data, enemy_data)                              \
	class name : public level::Level {                                         \
		constexpr mdspan::Span2d<const unsigned char> map() override {                    \
			return mdspan::Span2d<const unsigned char>(map_data);                         \
		}                                                                      \
		constexpr std::span<const unit::Unit> enemies() override { \
			return std::span(enemy_data, sizeof(enemy_data) / sizeof(unit::Unit)); \
		}                                                                      \
	};

void load_level(Level &level);

}; // namespace level
