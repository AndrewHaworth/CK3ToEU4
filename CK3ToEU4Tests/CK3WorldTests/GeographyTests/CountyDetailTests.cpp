#include "../CK3ToEU4/Source/CK3World/Geography/CountyDetail.h"
#include "../CK3ToEU4/Source/CK3World/Religions/Rites.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK3World_CountyDetailTests, loadValuesDefaultToBlank)
{
	std::stringstream input;
	const CK3::CountyDetail detail(input);

	ASSERT_FALSE(detail.getDevelopment());
	ASSERT_FALSE(detail.getCulture().first);
	ASSERT_FALSE(detail.getFaith().first);
}

TEST(CK3World_CountyDetailTests, detailPrimitivesCanBeLoaded)
{
	std::stringstream input;
	input << "development = 7\n";
	input << "culture = 9\n";
	input << "faith = 5\n";

	const CK3::CountyDetail detail(input);

	ASSERT_EQ(7, detail.getDevelopment());
	ASSERT_EQ(9, detail.getCulture().first);
	ASSERT_EQ(5, detail.getFaith().first);
}

TEST(CK3World_CountyDetailTests, ck3_120_riteResolvesIntoFaith)
{
	std::stringstream input;
	input << "culture = 374 rite = 200\n";
	CK3::CountyDetail detail(input);

	std::stringstream riteInput;
	riteInput << "database = { 200 = { faith = 13 } }\n";
	const CK3::Rites rites(riteInput);

	detail.resolveRite(rites);
	ASSERT_EQ(13, detail.getFaith().first);
	ASSERT_EQ(374, detail.getCulture().first);
}
