#include "PutOption.h"

PutOption::PutOption(double expiry, double strike)
	: EuropeanVanillaOption(expiry, strike)
{
}

double PutOption::payoff(double spot) const
{
	if (spot < getStrike())
		return getStrike() - spot;
	else
		return 0.0;
}

EuropeanVanillaOption::optionType PutOption::GetOptionType() const
{
	return EuropeanVanillaOption::optionType::put;
}

