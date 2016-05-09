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
bool read_sign(Iterator& it, Iterator end) {
	if (it == end) {
		return false;
	}
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


template<typename Integer, Integer Base>
constexpr unsigned safe_rounds_pos() {
	auto max = std::numeric_limits<Integer>::max();
	auto i = 0u;
	while (max >= Base) {
		max /= Base;
		++i;
	}
	return i;
}
template<typename Integer, Integer Base>
constexpr unsigned safe_rounds_neg() {
	auto min = std::numeric_limits<Integer>::min();
	auto i = 0u;
	while (min <= -Base) {
		min /= Base;
		++i;
	}
	return i;
}

template<unsigned Base, typename Integer, typename Iterator>
inline Integer parse_positive(Integer current, Iterator it, Iterator end, std::integral_constant<unsigned, 0>) {
	if (it == end) {return current;}
	constexpr const auto max = std::numeric_limits<Integer>::max();
	const auto digit = to_digit<Integer, Base>(*it);
	++it;
	if (max / Integer{Base} < current or max - digit < (current *= Integer{Base}) or it != end) {
		throw out_of_bounds_failure{"integer to big"};
	}
	current += digit;
	return current;
}

template<unsigned Base, typename Integer, unsigned Rounds, typename Iterator>
inline Integer parse_positive(Integer current, Iterator it, Iterator end, std::integral_constant<unsigned, Rounds>) {
	if (it == end) {return current;}
	current *= Base;
	current += to_digit<Integer, Base>(*it);
	++it;
	return parse_positive<Base>(current, it, end, std::integral_constant<unsigned, Rounds - 1>{});
}


template<unsigned Base, typename Integer, typename Iterator>
inline Integer parse_negative(Integer current, Iterator it, Iterator end, std::integral_constant<unsigned, 0>) {
	if (it == end) {return current;}
	constexpr const auto min = std::numeric_limits<Integer>::min();
	const auto digit = to_digit<Integer, Base>(*it);
	++it;
	if (min / Integer{Base} > current or min + digit > (current *= Integer{Base}) or it != end) {
		throw out_of_bounds_failure{"integer to small"};
	}
	current -= digit;
	return current;
}

template<unsigned Base, typename Integer, unsigned Rounds, typename Iterator>
inline Integer parse_negative(Integer current, Iterator it, Iterator end, std::integral_constant<unsigned, Rounds>) {
	if (it == end) {return current;}
	current *= Base;
	current -= to_digit<Integer, Base>(*it);
	++it;
	return parse_negative<Base>(current, it, end, std::integral_constant<unsigned, Rounds - 1>{});
}

template <typename Integer, unsigned Base, typename Iterator>
Integer s_to_integer(Iterator first, Iterator last, std::true_type /*signed*/) {
	const auto negative = read_sign(first, last);
	if (first == last) {
		throw invalid_input_failure{"no digits"};
	}
	if (negative) {
		return parse_negative<Base>(Integer{}, first, last,
				std::integral_constant<unsigned, safe_rounds_neg<Integer, Base>()>{});
	} else { // positive
		return parse_positive<Base>(Integer{}, first, last,
				std::integral_constant<unsigned, safe_rounds_pos<Integer, Base>()>{});
	}
}
template <typename Integer, unsigned Base, typename Iterator>
Integer s_to_integer(Iterator first, Iterator last, std::false_type /*unsigned*/) {
	const auto negative = read_sign(first, last);
	if (first == last) {
		throw invalid_input_failure{"no digits"};
	}
	const auto res = parse_positive<Base>(Integer{}, first, last,
			std::integral_constant<unsigned, safe_rounds_pos<Integer, Base>()>{});
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
