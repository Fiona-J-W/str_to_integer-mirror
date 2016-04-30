
#include <catch.hpp>

#include "sti/sti.hpp"

#include <chrono>
#include <iostream>

namespace {
void stoi_times(const std::string& str, unsigned n) {
	{
		auto integer = int{};
		auto begin = std::chrono::steady_clock::now();
		for(auto i = 0u; i < n; ++i) {
			integer ^= sti::str_to<int>(str);
		}
		auto end = std::chrono::steady_clock::now();
		std::cout << "str_to<...> in " << (end-begin).count() << ": " << integer << "\n";
	}
	{
		auto integer = int{};
		auto begin = std::chrono::steady_clock::now();
		for(auto i = 0u; i < n; ++i) {
			integer ^= stoi(str);
		}
		auto end = std::chrono::steady_clock::now();
		std::cout << "stoi        in " << (end-begin).count() << ": " << integer << "\n";
	}
}
} // namespace

TEST_CASE("to_int-performance", "[.performance]") {
	stoi_times("1", 1000000);
	stoi_times("10", 1000000);
	stoi_times("-10", 1000000);
	stoi_times("1000", 100000);
	stoi_times("10000", 1000000);
	stoi_times("100000", 10000000);
	stoi_times("1000000", 10000000);
	stoi_times("-1000000", 10000000);
}
