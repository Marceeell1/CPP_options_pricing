#pragma once
#include "EuropeanVanillaOption.h"

// European Call option
class CallOption : public EuropeanVanillaOption
{
public:
	CallOption(double expiry, double strike);

	// Payoff: max(S - K, 0)
	double payoff(double spot) const override;

	// Returns option type (call)
	optionType GetOptionType() const override;
};