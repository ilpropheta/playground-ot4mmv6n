// { autofold
#include "3rdparty/Catch.h"
#include "ver6/MicroUrlService.h"
// }

TEST_CASE("Check service's visitation works", "[visit]")
{	
	MicroUrlService service;

	std::vector<std::string> urls = { "https://google.com", "https://italiancpp.org", "https://coding-gym.org" };

	for (auto& url : urls)
		service.MakeMicroUrl(url);

	std::vector<std::string> actualUrls;
	service.VisitUrls([&](auto& urlInfo){
		actualUrls.push_back(urlInfo.OriginalUrl);
	});

	// assert that urls and actualUrls have the same elements, the order does not matter
	REQUIRE(true == std::is_permutation(begin(urls), end(urls), begin(actualUrls), end(actualUrls)));
}