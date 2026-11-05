#include "../CK3ToEU4/Source/CK3World/Religions/Faith.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK3World_FaithTests, faithIDLoads)
{
	std::stringstream input;
	const CK3::Faith faith(input, 42);

	ASSERT_EQ(42, faith.getID());
}

TEST(CK3World_FaithTests, loadValuesDefaultToBlank)
{
	std::stringstream input;
	const CK3::Faith faith(input, 1);

	ASSERT_TRUE(faith.getName().empty());
	ASSERT_TRUE(faith.getDoctrines().empty());
	ASSERT_FALSE(faith.getColor());
	ASSERT_FALSE(faith.getReligion().first);
	ASSERT_TRUE(faith.getCustomName().empty());
	ASSERT_TRUE(faith.getCustomAdj().empty());
	ASSERT_TRUE(faith.getDescription().empty());
	ASSERT_TRUE(faith.getTemplate().empty());
	ASSERT_TRUE(faith.getIconPath().empty());
	ASSERT_TRUE(!faith.getReformedFlag());
}

TEST(CK3World_FaithTests, faithPrimitivesCanBeLoaded)
{
	std::stringstream input;
	input << "tag=\"akom_pagan\"\n";
	input << "color=rgb { 229 178 76 }icon=\"i\\c\\on_path.dds\"\n";
	input << "doctrine=\"tenet_adorcism\"\n";
	input << "doctrine=\"doctrine_monogamy\"\n";
	input << "doctrine=\"doctrine_deviancy_shunned\"\n";
	input << "religion = 7\n";
	input << "name = \"Custom Name\"\n";
	input << "adjective = \"Custom Adj\"\n";
	input << "desc = \"Custom Desc\"\n";
	input << "icon = \"gfx/icon.dds\"\n";
	input << "template = \"catholic\"\n";
	input << "variables = { \"a bunch of nonsense \nreally\"}";

	const CK3::Faith faith(input, 42);

	ASSERT_EQ("akom_pagan", faith.getName());
	ASSERT_EQ(commonItems::Color(std::array<int, 3>{229, 178, 76}), faith.getColor());
	ASSERT_EQ(3, faith.getDoctrines().size());
	ASSERT_EQ(1, faith.getDoctrines()[0] == "tenet_adorcism");
	ASSERT_EQ(1, faith.getDoctrines()[1] == "doctrine_monogamy");
	ASSERT_EQ(1, faith.getDoctrines()[2] == "doctrine_deviancy_shunned");
	ASSERT_EQ(7, faith.getReligion().first);
	ASSERT_EQ("Custom Name", faith.getCustomName());
	ASSERT_EQ("Custom Adj", faith.getCustomAdj());
	ASSERT_EQ("Custom Desc", faith.getDescription());
	ASSERT_EQ("gfx/icon.dds", faith.getIconPath());
	ASSERT_EQ("catholic", faith.getTemplate());
	ASSERT_FALSE(faith.getReformedFlag());
}

TEST(CK3World_FaithTests, ck3_120_tenetsLoadAsDoctrinesInOrder)
{
	std::stringstream input;
	input << "tag=\"akom_pagan\"\n";
	input << "tenets={ { tenet=\"tenet_ignored_nested\" status=core } }\n";
	input << "tenet=\"tenet_adorcism\"\n";
	input << "tenet=\"tenet_ancestor_worship\"\n";
	input << "doctrine=\"doctrine_monogamy\"\n";

	const CK3::Faith faith(input, 1);

	ASSERT_EQ(3, faith.getDoctrines().size());
	EXPECT_EQ("tenet_adorcism", faith.getDoctrines()[0]);
	EXPECT_EQ("tenet_ancestor_worship", faith.getDoctrines()[1]);
	EXPECT_EQ("doctrine_monogamy", faith.getDoctrines()[2]);
}

TEST(CK3World_FaithTests, ck3_120_faithTypeBecomesTemplateForCustomFaiths)
{
	std::stringstream input;
	input << "faith_type=\"catholic\"\n";
	input << "tag=\"dynamic_faith_108\"\n";

	const CK3::Faith faith(input, 1);

	EXPECT_EQ("catholic", faith.getTemplate());
}

TEST(CK3World_FaithTests, ck3_120_faithTypeMatchingOwnTagIsNotATemplate)
{
	std::stringstream input;
	input << "faith_type=\"akom_pagan\"\n";
	input << "tag=\"akom_pagan\"\n";

	const CK3::Faith faith(input, 1);

	EXPECT_TRUE(faith.getTemplate().empty());
}

TEST(CK3World_FaithTests, explicitTemplateWinsOverFaithType)
{
	std::stringstream input;
	input << "template=\"orthodox\"\n";
	input << "faith_type=\"catholic\"\n";
	input << "tag=\"dynamic_faith_1\"\n";

	const CK3::Faith faith(input, 1);

	EXPECT_EQ("orthodox", faith.getTemplate());
}
