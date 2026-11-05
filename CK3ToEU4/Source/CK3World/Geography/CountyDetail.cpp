#include "CountyDetail.h"
#include "../Religions/Rites.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"

CK3::CountyDetail::CountyDetail(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK3::CountyDetail::registerKeys()
{
	registerKeyword("development", [this](const std::string& unused, std::istream& theStream) {
		development = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream) {
		culture = std::make_pair(commonItems::singleLlong(theStream).getLlong(), nullptr);
		if (culture.first == 4294967295)
			culture.first = 0;
	});
	registerKeyword("faith", [this](const std::string& unused, std::istream& theStream) {
		faith = std::make_pair(commonItems::singleLlong(theStream).getLlong(), nullptr);
		if (faith.first == 4294967295)
			faith.first = 0;
	});
	registerKeyword("rite", [this](const std::string& unused, std::istream& theStream) {
		// CK3 1.20+ stores the county's rite instead of its faith. Resolved into a faith via resolveRite().
		rite = commonItems::singleLlong(theStream).getLlong();
		if (*rite == 4294967295)
			rite.reset();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void CK3::CountyDetail::resolveRite(const Rites& rites)
{
	if (!rite)
		return;
	if (const auto& faithID = rites.getFaithForRite(*rite))
		faith = std::make_pair(*faithID, nullptr);
}
