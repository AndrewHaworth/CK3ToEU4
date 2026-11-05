#include "Rites.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

CK3::Rites::Rites(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK3::Rites::registerKeys()
{
	registerKeyword("database", [this](std::istream& theStream) {
		for (const auto& [riteID, faithID]: Rites(theStream).getRiteFaiths())
			riteFaiths.emplace(riteID, faithID);
	});
	registerRegex(R"(\d+)", [this](const std::string& riteID, std::istream& theStream) {
		std::optional<long long> faithID;
		commonItems::parser riteParser;
		riteParser.registerKeyword("faith", [&faithID](std::istream& riteStream) {
			faithID = commonItems::getLlong(riteStream);
		});
		riteParser.IgnoreUnregisteredItems();
		riteParser.parseStream(theStream);
		if (faithID && *faithID != 4294967295)
			riteFaiths.emplace(std::stoll(riteID), *faithID);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<long long> CK3::Rites::getFaithForRite(const long long riteID) const
{
	if (const auto& itr = riteFaiths.find(riteID); itr != riteFaiths.end())
		return itr->second;
	return std::nullopt;
}
