// { autofold
#include "3rdparty/Catch.h"
#include "ver4/MicroUrlService.h"
// }

TEST_CASE("Checking stats", "Let's use structure bindings [binding]")
{	
	const auto urlToShorten = "http://italiancpp.org/win100million.believeme";
	MicroUrlService service;
	auto microUrl = service.MakeMicroUrl(urlToShorten);
	
	auto urlInfo = service.Stats(microUrl);
	auto& original = urlInfo.OriginalUrl;
	auto& micro = urlInfo.MicroUrl;
	auto& clicks = urlInfo.Clicks;
	
	REQUIRE (original == urlToShorten);
	REQUIRE (micro == microUrl);
	REQUIRE (clicks == 0);
}