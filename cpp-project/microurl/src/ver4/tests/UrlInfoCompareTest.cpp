// { autofold
#include "3rdparty/Catch.h"
#include "ver4/MicroUrlService.h"
// }

TEST_CASE("Comparing UrlInfo", "Can you write operator< for UrlInfo? [less]")
{
	{
		UrlInfo url1{ "http://google.com", "url1", 0 };
		UrlInfo url2{ "http://google.com", "url1", 0 };
		REQUIRE(!(url1 < url2));
	}

	{
		UrlInfo url1{ "http://google.com", "url2", 0 };
		UrlInfo url2{ "http://google.com", "url1", 0 };
		REQUIRE(url2 < url1);
	}

	{
		UrlInfo url1{ "http://google.com", "url1", 0 };
		UrlInfo url2{ "http://google.com", "url2", 2 };
		REQUIRE(url1 < url2);
	}
}