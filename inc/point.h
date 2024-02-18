#pragma once

#include <concepts>

namespace point {

template <typename T>
concept NumericType = std::integral<T> || std::floating_point<T>;

template <NumericType T> struct Point {
	using Self = Point<T>;

	T x{0};
	T y{0};

	[[nodiscard]] constexpr Self operator+(Point<T> const &rhs) const noexcept {
		return Point{

	[[nodiscard]] constexpr Self operator+(Self const &rhs) const noexcept {
		return Self{
			.x = (T)(this->x + rhs.x),
			.y = (T)(this->y + rhs.y),
		};
	}

	[[nodiscard]] constexpr Self operator-(Self const &rhs) const noexcept {
		return Self{
			.x = (T)(this->x - rhs.x),
			.y = (T)(this->y - rhs.y),
		};
	}

	[[nodiscard]] constexpr Self operator*(T const &rhs) const noexcept {
		return Self{
			.x = (T)(this->x * rhs),
			.y = (T)(this->y * rhs),
		};
	}

	[[nodiscard]] constexpr Self operator/(T const &rhs) const noexcept {
		return Self{
			.x = (T)(this->x / rhs),
			.y = (T)(this->y / rhs),
		};
	}

	constexpr Self &operator+=(Self const &rhs) noexcept {
		*this = *this + rhs;
		return *this;
	}

	constexpr Self &operator-=(Self const &rhs) noexcept {
		*this = *this - rhs;
		return *this;
	}

	constexpr Self &operator*=(T const &rhs) noexcept {
		*this = *this * rhs;
		return *this;
	}

	constexpr Self &operator/=(T const &rhs) noexcept {
		*this = *this / rhs;
		return *this;
	}
};

} // namespace point
