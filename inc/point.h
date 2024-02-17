#pragma once

#include <concepts>

namespace point {

template <typename T>
concept NumericType = std::integral<T> || std::floating_point<T>;

template <NumericType T> struct Point {
	T x{0};
	T y{0};

	[[nodiscard]] constexpr Point<T> operator+(Point<T> const &rhs) noexcept {
		return Point{
			.x = (T)(this->x + rhs.x),
			.y = (T)(this->y + rhs.y),
		};
	}

	[[nodiscard]] constexpr Point<T> operator-(Point<T> const &rhs) noexcept {
		return Point{
			.x = (T)(this->x - rhs.x),
			.y = (T)(this->y - rhs.y),
		};
	}

	[[nodiscard]] constexpr Point<T> operator*(T const &rhs) noexcept {
		return Point{
			.x = (T)(this->x * rhs),
			.y = (T)(this->y * rhs),
		};
	}

	[[nodiscard]] constexpr Point<T> operator/(T const &rhs) noexcept {
		return Point{
			.x = (T)(this->x / rhs),
			.y = (T)(this->y / rhs),
		};
	}

	constexpr Point<T> &operator+=(Point<T> const &rhs) noexcept {
		*this = *this + rhs;
		return *this;
	}

	constexpr Point<T> &operator-=(Point<T> const &rhs) noexcept {
		*this = *this - rhs;
		return *this;
	}

	constexpr Point<T> &operator*=(T const &rhs) noexcept {
		*this = *this * rhs;
		return *this;
	}

	constexpr Point<T> &operator/=(T const &rhs) noexcept {
		*this = *this / rhs;
		return *this;
	}
};

} // namespace point
