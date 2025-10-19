#include "CallOption.h"

CallOption::CallOption(double expiry, double strike)
	: EuropeanVanillaOption(expiry, strike)
{
}

double CallOption::payoff(double spot) const
{
	if(spot > getStrike())
		return spot - getStrike();
	else
		return 0.0;
}

EuropeanVanillaOption::optionType CallOption::GetOptionType() const
{
	return EuropeanVanillaOption::optionType::call;
}