#pragma once

#include <array>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <type_traits>

namespace mdspan {

template <typename T> class Span2d {
	T *const data;

  public:
	size_t const width;
	size_t const height;

	/// Y, X
	constexpr T &operator[](size_t i, size_t j) const {
		return this->data[j + i * this->width];
	}

	/// Y, X
	constexpr T &at(size_t i, size_t j) const {
		if (j >= this->width || i >= this->height) {
			std::terminate();
		}
		return this[i, j];
	}

	constexpr T *begin() const { return this->data; }
	constexpr T const *cbegin() const { return (T const *)this->begin(); }

	constexpr T *end() const {
		return this->data + this->width * this->height;
	}
	constexpr T const *cend() const { return (T const *)this->end(); }

	template <size_t W, size_t H>
	constexpr Span2d(std::array<std::array<T, W>, H> const &arr)
		: data((T *)arr.data())
		, width(W)
		, height(H) {}

	template <size_t W, size_t H>
	constexpr Span2d(
		std::array<std::array<std::remove_const_t<T>, W>, H> const &arr
	)
		: data((T *)arr.data())
		, width(W)
		, height(H) {}

	constexpr Span2d()
		: data(nullptr)
		, width(0)
		, height(0) {}
};

} // namespace mdspan
