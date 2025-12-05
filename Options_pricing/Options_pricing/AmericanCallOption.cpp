#include "AmericanCallOption.h"
#include <algorithm>

AmericanCallOption::AmericanCallOption(double expiry, double strike)
    : AmericanOption(expiry, strike){}

double AmericanCallOption::payoff(double spot) const {
    return std::max(spot - getStrike(), 0.0);
}

AmericanCallOption::~AmericanCallOption() = default;