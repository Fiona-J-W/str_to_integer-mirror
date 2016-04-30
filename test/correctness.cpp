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
