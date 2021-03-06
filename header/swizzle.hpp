#pragma once
#include "scalar.hpp"
#include "vector-usage.hpp"

#include <exception>

namespace bf {
namespace math {
template <typename T, std::size_t STORAGE, std::size_t N, std::size_t... Indices>
class swizzle_storage {
	static_assert(sizeof...(Indices) == N, "invalid swizzle size.");
private:
	constexpr static std::size_t index_array[] = { Indices... };
	typename scalar_traits<T>::scalar_type value[STORAGE];

	static std::size_t swizzle_index(std::size_t i) {
		return index_array[i];
	}

public:
	T& operator [] (std::size_t i) {
		return *reinterpret_cast<T*>(&value[swizzle_index(i)]);
	}

	const T& operator [] (std::size_t i) const {
		return *reinterpret_cast<const T*>(&value[swizzle_index(i)]);
	}

	swizzle_storage& operator = (const T& operand) {
		for (auto i = 0; i < N; ++i)
			(*this)[i] = operand;

		return *this;
	}

	template <typename T2, typename S2>
	swizzle_storage& operator = (const vector_usage<T2, N, S2>& operand) {
		vector<T, N> temp;
		for (auto i = 0; i < N; ++i)
			temp[i] = operand[i];

		for (auto i = 0; i < N; ++i)
			(*this)[i] = temp[i];

		return *this;
	}
};

template <typename T, std::size_t STORAGE, std::size_t N, std::size_t... Indices>
class swizzle : public vector_plural_usage<T, N, swizzle_storage<T, STORAGE, N, Indices...>> {
public:
	swizzle& operator = (const T& operand) {
		swizzle_storage<T, STORAGE, N, Indices...>::operator=(operand);
		return *this;
	}

	template<typename T2, typename S2>
	swizzle& operator = (const vector_usage<T2, N, S2>& operand) {
		swizzle_storage<T, STORAGE, N, Indices...>::operator=(operand);
		return *this;
	}
};

template <typename T, std::size_t STORAGE, std::size_t... Indices>
class swizzle<T, STORAGE, 1, Indices...> : public vector_usage<T, 1, swizzle_storage<T, STORAGE, 1, Indices...>>{
public:
	swizzle& operator = (const T& operand) {
		swizzle_storage<T, STORAGE, 1, Indices...>::operator=(operand);
		return *this;
	}

	template<typename T2, typename S2>
	swizzle& operator = (const vector_usage<T2, 1, S2>& operand) {
		swizzle_storage<T, STORAGE, 1, Indices...>::operator=(operand);
		return *this;
	}

	operator T() const {
		return (*this)[0];
	}
};

}
}