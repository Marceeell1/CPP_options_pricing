#include "CallOption.h"

// Constructor
CallOption::CallOption(double expiry, double strike)
	: EuropeanVanillaOption(expiry, strike)
{
}

// Payoff function: max(S - K, 0)
double CallOption::payoff(double spot) const
{
	if (spot > getStrike())
		return spot - getStrike();
	else
		return 0.0;
}

// Returns option type (call)
EuropeanVanillaOption::optionType CallOption::GetOptionType() const
{
	return EuropeanVanillaOption::optionType::call;
}