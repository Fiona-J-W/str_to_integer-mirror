#ifndef STI_STI_HPP
#define STI_STI_HPP

#include <stdexcept>
#include <type_traits>
#include <string>
#include <limits>

namespace sti {

class conversion_failure : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class out_of_bounds_failure : public conversion_failure {
	using conversion_failure::conversion_failure;
};

class invalid_input_failure : public conversion_failure {
	using conversion_failure::conversion_failure;
};

namespace impl {

template <typename Char>
struct char_literals;

template <>
struct char_literals<char> {
	constexpr static const char zero = '0';
	constexpr static const char small_a = 'a';
	constexpr static const char big_a = 'A';
	constexpr static const char plus = '+';
	constexpr static const char minus = '-';
};
template <>
struct char_literals<wchar_t> {
	constexpr static const wchar_t zero = L'0';
	constexpr static const wchar_t small_a = L'a';
	constexpr static const wchar_t big_a = L'A';
	constexpr static const wchar_t plus = L'+';
	constexpr static const wchar_t minus = L'-';
};
template <>
struct char_literals<char16_t> {
	constexpr static const char16_t zero = u'0';
	constexpr static const char16_t small_a = u'a';
	constexpr static const char16_t big_a = u'A';
	constexpr static const char16_t plus = u'+';
	constexpr static const char16_t minus = u'-';
};
template <>
struct char_literals<char32_t> {
	constexpr static const char32_t zero = U'0';
	constexpr static const char32_t small_a = U'a';
	constexpr static const char32_t big_a = U'A';
	constexpr static const char32_t plus = U'+';
	constexpr static const char32_t minus = U'-';
};

template <typename Integer, unsigned Base, typename Char>
Integer to_digit(Char c, std::false_type) {
	constexpr const auto c0 = char_literals<Char>::zero;
	constexpr const auto c9 = Char{c0 + 9};
	constexpr const auto ca = char_literals<Char>::small_a;
	constexpr const auto cm = Char{ca + Base - 11};
	constexpr const auto cA = char_literals<Char>::big_a;
	constexpr const auto cM = Char{cA + Base - 11};
	if (c0 <= c and c <= c9) {
		return static_cast<Integer>(c - c0);
	}
	if (ca <= c and c <= cm) {
		return static_cast<Integer>(c - ca + 10);
	}
	if (cA <= c and c <= cM) {
		return static_cast<Integer>(c - cA + 10);
	}
	throw invalid_input_failure{std::string{"bad digit: "} +
	                            std::to_string(static_cast<std::uint32_t>(c)) + ", base = " +
	                            std::to_string(Base)};
}

template <typename Integer, unsigned Base, typename Char>
Integer to_digit(Char c, std::true_type) {
	constexpr const Char c0 = char_literals<Char>::zero;
	constexpr const Char cm = c0 + Base - 1;
	if (c0 <= c and c <= cm) {
		return static_cast<Integer>(c - c0);
	}
	throw invalid_input_failure{std::string{"bad digit: "} +
	                            std::to_string(static_cast<std::uint32_t>(c)) + ", base = " +
	                            std::to_string(Base)};
}

template <typename Integer, unsigned Base, typename Char>
Integer to_digit(Char c) {
	static_assert(Base >= 2u and Base <= 10u + ('Z' - 'A'), "");
	return to_digit<Integer, Base>(c, std::integral_constant<bool, (Base <= 10u)>{});
}

/**
 * Swallows one '+' or '-' if that is what the iterator
 * points too and returns whether a '-' was swallowed.
 */
template <typename Iterator>
bool read_sign(Iterator& it) {
	const auto c = *it;
	using char_type = std::decay_t<decltype(c)>;
	if (c == char_literals<char_type>::plus) {
		++it;
		return false;
	}
	if (c == char_literals<char_type>::minus) {
		++it;
		return true;
	}
	return false;
}

template <typename Integer, unsigned Base, typename Iterator>
Integer s_to_integer(Iterator first, Iterator last, std::true_type /*signed*/) {
	const auto negative = read_sign(first);
	if (first == last) {
		throw invalid_input_failure{"no digits"};
	}
	auto res = Integer{};
	constexpr const auto min = std::numeric_limits<Integer>::min();
	constexpr const auto max = std::numeric_limits<Integer>::max();
	// "return negative ? -res : res;" doesn't work for min, so
	// we have to use a conditional here
	if (negative) {
		while (first != last) {
			const auto digit = to_digit<Integer, Base>(*first);
			++first;
			// res is already multiplied in the condition, because GCC
			// seems to be unable to recognize a semantically identical
			// multiplicaton later on as something it could remove
			if (min / Integer{Base} > res or min + digit > (res *= Integer{Base})) {
				throw out_of_bounds_failure{"integer to small"};
			}
			res -= digit;
		}
	} else { // positive
		while (first != last) {
			const auto digit = to_digit<Integer, Base>(*first);
			++first;
			if (max / Integer{Base} < res or max - digit < (res *= Integer{Base})) {
				throw out_of_bounds_failure{"integer to big"};
			}
			res += digit;
		}
	}
	return res;
}

template <typename Integer, unsigned Base, typename Iterator>
Integer s_to_integer(Iterator first, Iterator last, std::false_type /*unsigned*/) {
	const auto negative = read_sign(first);
	if (first == last) {
		throw invalid_input_failure{"no digits"};
	}
	constexpr const auto max = std::numeric_limits<Integer>::max();
	auto res = Integer{};
	while (first != last) {
		const auto digit = to_digit<Integer, Base>(*first);
		++first;
		if (max / Base < res or max - digit < (res *= Integer{Base})) {
			throw out_of_bounds_failure{"integer to big (2)"};
		}
		res += digit;
	}
	if (negative and res != Integer{0}) {
		throw out_of_bounds_failure{"unsigned integers cannot hold negative values"};
	}
	return res;
}

} // namespace impl

template <typename Integer, unsigned Base = 10u, typename Iterator>
Integer str_to(Iterator first, Iterator last) {
	return impl::s_to_integer<Integer, Base>(first, last, std::is_signed<Integer>{});
}

template <typename Integer, unsigned Base = 10u, typename String>
Integer str_to(const String& str) {
	using std::begin;
	using std::end;
	return str_to<Integer, Base>(begin(str), end(str));
}

template <typename Integer, unsigned Base = 10u, typename Char>
Integer str_to(const Char* str) {
	return str_to<Integer, Base>(str, str + std::char_traits<Char>::length(str));
}
}

#endif
