
#include <catch.hpp>

#include "sti/sti.hpp"

TEST_CASE("decimal int") {
	CHECK(sti::str_to<int>("23") == 23);
	CHECK(sti::str_to<int>("-42") == -42);
	CHECK(sti::str_to<int>("-2147483648") == -2147483648);
	CHECK(sti::str_to<int>("-2147483647") == -2147483647);
	CHECK(sti::str_to<int>("+2147483647") == 2147483647);
	CHECK(sti::str_to<int>("0") == 0);
	CHECK(sti::str_to<int>("-0") == 0);
	CHECK(sti::str_to<int>("+0") == 0);
	CHECK(sti::str_to<int>("-1") == -1);
	CHECK(sti::str_to<int>("+1") == 1);
	CHECK(sti::str_to<int>("1") == 1);
}
