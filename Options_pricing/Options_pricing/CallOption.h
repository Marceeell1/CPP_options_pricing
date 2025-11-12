#pragma once
#include "EuropeanVanillaOption.h"

// Represents a European Call option.
class CallOption : public EuropeanVanillaOption
{
public:
	// Constructor: sets expiry and strike.
	CallOption(double expiry, double strike);

	// Payoff: max(S - K, 0)
	double payoff(double spot) const override;

	// Returns option type (call).
	optionType GetOptionType() const override;
};
