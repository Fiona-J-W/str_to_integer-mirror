#include "sti/sti.hpp"

#include <catch.hpp>
#include <cstdint>

TEST_CASE("decimal std::int32_t") {
	CHECK(sti::str_to<std::int32_t>("23") == 23);
	CHECK(sti::str_to<std::int32_t>("-42") == -42);
	CHECK(sti::str_to<std::int32_t>("-2147483648") == -2147483648);
	CHECK(sti::str_to<std::int32_t>("-2147483647") == -2147483647);
	CHECK(sti::str_to<std::int32_t>("+2147483647") == 2147483647);
	CHECK(sti::str_to<std::int32_t>("0") == 0);
	CHECK(sti::str_to<std::int32_t>("-0") == 0);
	CHECK(sti::str_to<std::int32_t>("+0") == 0);
	CHECK(sti::str_to<std::int32_t>("-1") == -1);
	CHECK(sti::str_to<std::int32_t>("+1") == 1);
	CHECK(sti::str_to<std::int32_t>("1") == 1);
	CHECK(sti::str_to<std::int32_t>("+001") == 1);

	CHECK_THROWS_AS(sti::str_to<std::int32_t>(""), sti::invalid_input_failure);
	CHECK_THROWS_AS(sti::str_to<std::int32_t>("+"), sti::invalid_input_failure);
	CHECK_THROWS_AS(sti::str_to<std::int32_t>("-"), sti::invalid_input_failure);
	CHECK_THROWS_AS(sti::str_to<std::int32_t>("23x"), sti::invalid_input_failure);
	CHECK_THROWS_AS(sti::str_to<std::int32_t>("y"), sti::invalid_input_failure);
	CHECK_THROWS_AS(sti::str_to<std::int32_t>("a"), sti::invalid_input_failure);
	CHECK_THROWS_AS(sti::str_to<std::int32_t>("A"), sti::invalid_input_failure);

	CHECK_THROWS_AS(sti::str_to<std::int32_t>("-2147483649"), sti::out_of_bounds_failure);
	CHECK_THROWS_AS(sti::str_to<std::int32_t>("-2147483658"), sti::out_of_bounds_failure);
	CHECK_THROWS_AS(sti::str_to<std::int32_t>("+2147483648"), sti::out_of_bounds_failure);
	CHECK_THROWS_AS(sti::str_to<std::int32_t>("2147483648"), sti::out_of_bounds_failure);

	const auto bad_str = std::string{'2', '\0', '3'};
	CHECK_THROWS_AS(sti::str_to<std::int32_t>(bad_str), sti::invalid_input_failure);
}

TEST_CASE("decimal std::uint32_t") {
	CHECK(sti::str_to<std::uint32_t>("23") == 23);
	CHECK(sti::str_to<std::uint32_t>("42") == 42);
	CHECK(sti::str_to<std::uint32_t>("4294967295") == 4294967295u);
	CHECK(sti::str_to<std::uint32_t>("+4294967295") == 4294967295u);
	CHECK(sti::str_to<std::uint32_t>("12345") == 12345);
	CHECK(sti::str_to<std::uint32_t>("0") == 0);
	CHECK(sti::str_to<std::uint32_t>("+0") == 0);
	CHECK(sti::str_to<std::uint32_t>("1") == 1);
	CHECK(sti::str_to<std::uint32_t>("+1") == 1);
	CHECK(sti::str_to<std::uint32_t>("+001") == 1);

	CHECK_THROWS_AS(sti::str_to<std::uint32_t>(""), sti::invalid_input_failure);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("+"), sti::invalid_input_failure);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("23x"), sti::invalid_input_failure);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("y"), sti::invalid_input_failure);
	CHECK_THROWS(sti::str_to<std::uint32_t>("-")); // fails with out of bounds instead of
	                                               // sti::invalid_input_failure); is this
	                                               // desirable?

	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("4294967296"), sti::out_of_bounds_failure);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("+4294967296"), sti::out_of_bounds_failure);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("42949672950"), sti::out_of_bounds_failure);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("+42949672950"), sti::out_of_bounds_failure);

	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("-1"), sti::out_of_bounds_failure);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("-42"), sti::out_of_bounds_failure);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>("-42949672950"), sti::out_of_bounds_failure);

	const auto bad_str = std::string{'2', '\0', '3'};
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>(bad_str), sti::invalid_input_failure);
}

TEST_CASE("hexadecimal std::int32_t") {
	CHECK((sti::str_to<std::int32_t, 16>("23")) == 0x23);
	CHECK((sti::str_to<std::int32_t, 16>("-42")) == -0x42);
	CHECK((sti::str_to<std::int32_t, 16>("-fffffff")) == -0xfffffff);
	CHECK((sti::str_to<std::int32_t, 16>("-80000000")) == -2147483648);
	CHECK((sti::str_to<std::int32_t, 16>("+a")) == 0xa);
	CHECK((sti::str_to<std::int32_t, 16>("+A")) == 0xa);
	CHECK((sti::str_to<std::int32_t, 16>("-a")) == -0xa);
	CHECK((sti::str_to<std::int32_t, 16>("-A")) == -0xa);
	CHECK((sti::str_to<std::int32_t, 16>("0")) == 0);
	CHECK((sti::str_to<std::int32_t, 16>("-0")) == 0);
	CHECK((sti::str_to<std::int32_t, 16>("+0")) == 0);
	CHECK((sti::str_to<std::int32_t, 16>("-1")) == -1);
	CHECK((sti::str_to<std::int32_t, 16>("+1")) == 1);
	CHECK((sti::str_to<std::int32_t, 16>("1")) == 1);
	CHECK((sti::str_to<std::int32_t, 16>("+001")) == 1);

	CHECK_THROWS_AS((sti::str_to<std::int32_t, 16>("")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::int32_t, 16>("+")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::int32_t, 16>("-")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::int32_t, 16>("23x")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::int32_t, 16>("g")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::int32_t, 16>("y")), sti::invalid_input_failure);

	CHECK_THROWS_AS((sti::str_to<std::int32_t, 16>("-80000001")), sti::out_of_bounds_failure);
	CHECK_THROWS_AS((sti::str_to<std::int32_t, 16>("80000000")), sti::out_of_bounds_failure);
}

TEST_CASE("hexadecimal std::uint32_t") {
	CHECK((sti::str_to<std::uint32_t, 16>("23")) == 0x23);
	CHECK((sti::str_to<std::uint32_t, 16>("42")) == 0x42);
	CHECK((sti::str_to<std::uint32_t, 16>("ffffffff")) == 0xffffffffu);
	CHECK((sti::str_to<std::uint32_t, 16>("+ffffffff")) == 0xffffffffu);
	CHECK((sti::str_to<std::uint32_t, 16>("12345")) == 0x12345);
	CHECK((sti::str_to<std::uint32_t, 16>("0")) == 0);
	CHECK((sti::str_to<std::uint32_t, 16>("+0")) == 0);
	CHECK((sti::str_to<std::uint32_t, 16>("1")) == 1);
	CHECK((sti::str_to<std::uint32_t, 16>("+1")) == 1);
	CHECK((sti::str_to<std::uint32_t, 16>("+001")) == 1);

	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("+")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("23x")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("g")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("y")), sti::invalid_input_failure);
	CHECK_THROWS((sti::str_to<std::uint32_t, 16>("-"))); // fails with out of bounds instead of
	                                               // sti::invalid_input_failure); is this
	                                               // desirable?

	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("100000000")), sti::out_of_bounds_failure);
	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("+100000000")), sti::out_of_bounds_failure);

	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("-1")), sti::out_of_bounds_failure);
	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("-42")), sti::out_of_bounds_failure);
	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>("-ffffffff")), sti::out_of_bounds_failure);

	const auto bad_str = std::string{'2', '\0', '3'};
	CHECK_THROWS_AS((sti::str_to<std::uint32_t, 16>(bad_str)), sti::invalid_input_failure);
}

TEST_CASE("binary std::int8_t") {
	CHECK((sti::str_to<std::int8_t, 2>("01")) == 1);
	CHECK((sti::str_to<std::int8_t, 2>("10")) == 2);
	CHECK((sti::str_to<std::int8_t, 2>("11")) == 3);
	CHECK((sti::str_to<std::int8_t, 2>("1111")) == 15);
	CHECK((sti::str_to<std::int8_t, 2>("-01")) == -1);
	CHECK((sti::str_to<std::int8_t, 2>("-10")) == -2);
	CHECK((sti::str_to<std::int8_t, 2>("-11")) == -3);
	CHECK((sti::str_to<std::int8_t, 2>("-1111")) == -15);
	CHECK_THROWS_AS((sti::str_to<std::int8_t, 2>("2")), sti::invalid_input_failure);
	CHECK_THROWS_AS((sti::str_to<std::int8_t, 2>("10000000")), sti::out_of_bounds_failure);
}

TEST_CASE("wchar_t misc") {
	CHECK(sti::str_to<std::int32_t>(L"-2147483647") == -2147483647);
	CHECK((sti::str_to<std::uint32_t, 16>(L"23")) == 0x23);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>(L"+42949672950"), sti::out_of_bounds_failure);
}

TEST_CASE("char16_t misc") {
	CHECK(sti::str_to<std::int32_t>(u"-2147483647") == -2147483647);
	CHECK((sti::str_to<std::uint32_t, 16>(u"23")) == 0x23);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>(u"+42949672950"), sti::out_of_bounds_failure);
}

TEST_CASE("char32_t misc") {
	CHECK(sti::str_to<std::int32_t>(U"-2147483647") == -2147483647);
	CHECK((sti::str_to<std::uint32_t, 16>(U"23")) == 0x23);
	CHECK_THROWS_AS(sti::str_to<std::uint32_t>(U"+42949672950"), sti::out_of_bounds_failure);
}

