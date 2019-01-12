// { autofold
#include "3rdparty/Catch.h"
#include "ver5/MicroUrlService.h"
// }

TEST_CASE("Is it always a good idea to replace const string& with string_view?", "This test is [subtle]")
{
	/*MicroUrlService service;
	auto microGoogleCom = service.MakeMicroUrl("http://google.com");
    
    	std::string buffer = microGoogleCom + "https://micro.url/bA";    
	std::string_view visitThis{ buffer.c_str(), microGoogleCom.size() };
	REQUIRE(service.ClickUrl(visitThis) == "http://google.com");*/
}
