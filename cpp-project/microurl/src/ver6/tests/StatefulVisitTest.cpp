// { autofold
#include "3rdparty/Catch.h"
#include "ver6/MicroUrlService.h"
#include <map>
#include <algorithm>
// }

struct UrlFrequencyVisitor
{
	void operator()(const UrlInfo& info)
	{
		m_freq[info.OriginalUrl]++;
	}

	std::string MostPopular() const
	{
		ptrdiff_t freq = 0;
		auto maxElemIt = begin(m_freq);
		auto curr = begin(m_freq);
		while (++curr != end(m_freq))
		{
			if (curr->second > maxElemIt->second)
			{
				maxElemIt = curr;
			}
		}
		return end(m_freq) != maxElemIt ? maxElemIt->first : std::string{};
	}

	ptrdiff_t StartingWith(std::string_view sv) const
	{
		auto beginPrefix = m_freq.lower_bound(sv);
		auto endPrefix = end(m_freq);
		auto first = beginPrefix;
		while (beginPrefix != end(m_freq))
		{
			if (first->first.compare(0, sv.size(), sv) != 0)
			{
				endPrefix = first;
				break;
			}
			++first;
		}
		return std::distance(beginPrefix, endPrefix);
	}

private:
	std::map<std::string, ptrdiff_t, std::less<>> m_freq;
};

TEST_CASE("Using Visit to count the most popular urls", "[count]")
{	
	MicroUrlService service;
	
	for (auto i=0; i<5; ++i)
		service.MakeMicroUrl("http://google.com");
	for (auto i = 0; i<10; ++i)
		service.MakeMicroUrl("http://italiancpp.org");
	for (auto i = 0; i<8; ++i)
		service.MakeMicroUrl("http://coding-gym.org");


	UrlFrequencyVisitor visitor;
	service.VisitUrls(std::ref(visitor));
	
	REQUIRE(visitor.MostPopular() == "http://italiancpp.org");
}

TEST_CASE("Counting the number of the italiancpp articles", "[prefix]")
{
	MicroUrlService service;

	service.MakeMicroUrl("google.com");
	service.MakeMicroUrl("italiancpp.org/2018/12/08/cppday18/");
	service.MakeMicroUrl("youborn.com");
	service.MakeMicroUrl("italiancpp.org/2018/06/24/5-years-of-italiancpp/");
	service.MakeMicroUrl("italiancpp.org/2017/07/04/itcppcon17/");
	service.MakeMicroUrl("italiancipipi.com");

	UrlFrequencyVisitor visitor;
	service.VisitUrls(std::ref(visitor));

	REQUIRE(visitor.StartingWith("italiancpp.org") == 3);
	REQUIRE(visitor.StartingWith("google") == 1);
}
