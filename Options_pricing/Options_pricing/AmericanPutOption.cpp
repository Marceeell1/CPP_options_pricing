#include "AmericanPutOption.h"
#include <algorithm>

AmericanPutOption::AmericanPutOption(double expiry, double strike)
	: AmericanOption(expiry, strike){}

double AmericanPutOption::payoff(double spot) const {
	return std::max(getStrike() - spot, 0.0);
}

AmericanPutOption::~AmericanPutOption() = default;