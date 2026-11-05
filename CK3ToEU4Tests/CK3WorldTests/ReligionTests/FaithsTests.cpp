#include "../../CK3ToEU4/Source/CK3World/Religions/Faith.h"
#include "../../CK3ToEU4/Source/CK3World/Religions/Faiths.h"
#include "../../CK3ToEU4/Source/CK3World/Religions/Religion.h"
#include "../../CK3ToEU4/Source/CK3World/Religions/Religions.h"
#include "../../CK3ToEU4/Source/CK3World/Religions/Rites.h"
#include "../../CK3ToEU4/Source/CK3World/Titles/Titles.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK3World_FaithsTests, FaithsDefaultToEmpty)
{
	std::stringstream input;
	const CK3::Faiths faiths(input);

	ASSERT_TRUE(faiths.getFaiths().empty());
}

TEST(CK3World_FaithsTests, FaithsCanBeLoaded)
{
	std::stringstream input;
	input << "13={tag=\"old_bon\"}\n";
	input << "15={tag=\"theravada\"}\n";

	const CK3::Faiths faiths(input);
	const auto& h1 = faiths.getFaiths().find(13);
	const auto& h2 = faiths.getFaiths().find(15);

	ASSERT_EQ("old_bon", h1->second->getName());
	ASSERT_EQ("theravada", h2->second->getName());
}

TEST(CK3World_FaithsTests, religionCanBeLinked)
{
	std::stringstream input;
	input << "13={tag=\"old_bon\"\n religion = 1}\n";
	input << "15={tag=\"theravada\"\n religion = 2}\n";
	CK3::Faiths faiths(input);

	std::stringstream input2;
	input2 << "1 = { tag=\"bon_religion\"}\n";
	input2 << "2 = { tag=\"buddhism_religion\"}\n";
	const CK3::Religions religions(input2);
	const CK3::Titles titles;
	faiths.linkReligions(religions, titles);

	const auto& f13 = faiths.getFaiths().find(13);
	const auto& f15 = faiths.getFaiths().find(15);

	ASSERT_EQ("bon_religion", f13->second->getReligion().second->getName());
	ASSERT_EQ("buddhism_religion", f15->second->getReligion().second->getName());
}

TEST(CK3World_FaithsTests, linkingMissingReligionThrowsException)
{
	std::stringstream input;
	input << "13={tag=\"old_bon\"\n religion = 1}\n";
	input << "15={tag=\"theravada\"\n religion = 2}\n";
	CK3::Faiths faiths(input);

	std::stringstream input2;
	input2 << "1 = { tag=\"bon_religion\"}\n";
	const CK3::Religions religions(input2);
	const CK3::Titles titles;

	ASSERT_THROW(faiths.linkReligions(religions, titles), std::runtime_error);
}

TEST(CK3World_FaithsTests, ck3_120_FaithsCanBeLoadedFromDatabaseBlock)
{
	std::stringstream input;
	input << "database={\n";
	input << "\t13={tag=\"old_bon\"}\n";
	input << "\t15={tag=\"theravada\"}\n";
	input << "}\n";

	const CK3::Faiths faiths(input);

	ASSERT_EQ(2, faiths.getFaiths().size());
	EXPECT_EQ("old_bon", faiths.getFaiths().at(13)->getName());
	EXPECT_EQ("theravada", faiths.getFaiths().at(15)->getName());
}

TEST(CK3World_RitesTests, ck3_120_RitesMapToTheirFaith)
{
	std::stringstream input;
	input << "database = {\n";
	input << "\t0 = { rite_type = \"roman_rite\" faith = 13 founder = 4294967295 data = { tag = \"roman_rite\" faith = 77 } }\n";
	input << "\t6 = { rite_type = \"byzantine_rite\" faith = 14 }\n";
	input << "\t7 = { faith = 4294967295 }\n";
	input << "}\n";
	const CK3::Rites rites(input);

	ASSERT_EQ(2, rites.getRiteFaiths().size());
	ASSERT_EQ(13, rites.getFaithForRite(0).value());
	ASSERT_EQ(14, rites.getFaithForRite(6).value());
	ASSERT_FALSE(rites.getFaithForRite(7));
	ASSERT_FALSE(rites.getFaithForRite(99));
}
