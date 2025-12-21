#pragma once
#include "EuropeanVanillaOption.h"

// European Put option
class PutOption : public EuropeanVanillaOption
{
public:
	PutOption(double expiry, double strike);

	// Payoff: max(K - S, 0)
	double payoff(double spot) const override;

	// Returns option type (put)
	optionType GetOptionType() const override;
};


