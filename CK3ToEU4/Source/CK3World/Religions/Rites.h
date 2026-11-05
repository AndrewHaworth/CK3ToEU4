#ifndef CK3_RITES_H
#define CK3_RITES_H
#include "Parser.h"
#include <map>

namespace CK3
{
// CK3 1.20+ splits faiths into rites. Characters and counties reference a rite, and each rite belongs to exactly one faith.
class Rites: commonItems::parser
{
  public:
	Rites() = default;
	explicit Rites(std::istream& theStream);

	[[nodiscard]] const auto& getRiteFaiths() const { return riteFaiths; }
	[[nodiscard]] std::optional<long long> getFaithForRite(long long riteID) const;

  private:
	void registerKeys();

	std::map<long long, long long> riteFaiths; // rite ID, faith ID
};
} // namespace CK3

#endif // CK3_RITES_H
