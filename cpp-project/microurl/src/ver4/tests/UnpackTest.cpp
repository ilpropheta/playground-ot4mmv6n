// { autofold
#include "3rdparty/Catch.h"
#include "ver4/MicroUrlService.h"
#include "ver4/IIdGenerator.h"
// }

template<typename F>
struct unpack_to
{
	unpack_to(F f) {}

	template<typename Arg>
	auto operator()(Arg&& arg) const
	{
		// ... implement this ...
		return 1; // this is just a fake value
	}
};

TEST_CASE("Designing and testing unpack_to", "[unpack]")
{	
	// message by Luke:
	auto funct = [](std::string, int) { };
	std::tuple<std::string, int> args{ "marco", 31 };
	std::apply(funct, args);
	// end message by luke
	
	std::vector<std::tuple<std::string, int, int>> expected = {
		{"google", 10, 20},
		{"italiancpp", 21, 80},
		{"coding-gym", 15, 100}
	};

	std::vector<std::tuple<std::string, int, int>> actual;
	std::for_each(begin(expected), end(expected), unpack_to([&](auto name, auto urls, auto clicks){
		actual.emplace_back(name, urls, clicks);
	}));

	REQUIRE(actual == expected);
	
	std::vector<std::pair<int, int>> expected2 = {
		{10, 20},
		{21, 80},
		{15, 100}
	};

	std::vector<std::pair<int, int>> actual2;
	std::for_each(begin(expected2), end(expected2), unpack_to([&](auto urls, auto clicks){
		actual2.emplace_back(urls, clicks);
	}));

	REQUIRE(actual2 == expected2);
}