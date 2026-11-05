#include "CK3ReligionScraper.h"
#include "CK3ReligionScraping.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"
#include <ranges>

void mappers::CK3ReligionScraper::scrapeCK3Religions(const commonItems::ModFilesystem& modFS)
{
	Log(LogLevel::Info) << "-> Scraping CK3 religion hierarchy.";
	registerKeys();
	ParseGameFolder("common/religion/religion_types", modFS, {".txt"}, false);
	ParseGameFolder("common/religion/religions", modFS, {".txt"}, false);
	clearRegisteredKeywords();

	// CK3 1.20+ no longer nests faiths inside religions. Each faith lives in faith_types and names its religion in faith_details.
	registerFaithTypeKeys();
	ParseGameFolder("common/religion/faith_types", modFS, {".txt"}, false);
	clearRegisteredKeywords();
	Log(LogLevel::Info) << "<> Loaded " << religionFaiths.size() << " religions.";
}

void mappers::CK3ReligionScraper::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& religionName, std::istream& theStream) {
		for (const auto& faith: CK3ReligionScraping(theStream).getFaithNames())
			religionFaiths[religionName].insert(faith);
	});
}

void mappers::CK3ReligionScraper::registerFaithTypeKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& faithName, std::istream& theStream) {
		std::string religionName;
		commonItems::parser detailsParser;
		detailsParser.registerKeyword("religion", [&religionName](std::istream& detailsStream) {
			religionName = commonItems::getString(detailsStream);
		});
		detailsParser.IgnoreUnregisteredItems();

		commonItems::parser faithParser;
		faithParser.registerKeyword("faith_details", [&detailsParser](std::istream& faithStream) {
			detailsParser.parseStream(faithStream);
		});
		faithParser.IgnoreUnregisteredItems();
		faithParser.parseStream(theStream);

		if (!religionName.empty())
			religionFaiths[religionName].insert(faithName);
	});
}

std::set<std::string> mappers::CK3ReligionScraper::getSimilarFaiths(const std::string& faith) const
{
	for (const auto& faiths: religionFaiths | std::views::values)
		if (faiths.contains(faith))
			return faiths;
	return {};
}
