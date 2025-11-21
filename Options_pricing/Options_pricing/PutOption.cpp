#include "PutOption.h"

// Constructor: initializes expiry and strike
PutOption::PutOption(double expiry, double strike)
	: EuropeanVanillaOption(expiry, strike)
{
}

// Payoff function: max(K - S, 0)
double PutOption::payoff(double spot) const
{
	if (spot < getStrike())
		return getStrike() - spot;
	else
		return 0.0;
}

// Returns option type (put)
EuropeanVanillaOption::optionType PutOption::GetOptionType() const
{
	return EuropeanVanillaOption::optionType::put;
}
