#include "AmericanCallOption.h"
#include <algorithm>

AmericanCallOption::AmericanCallOption(double expiry, double strike)
    : AmericanOption(expiry, strike){}

double AmericanCallOption::payoff(double spot) const {
    double strike = getStrike();
    
    if (strike < spot) {
        return spot - strike;
    } else {
        return 0.0;
    }
}

AmericanCallOption::~AmericanCallOption() = default;
