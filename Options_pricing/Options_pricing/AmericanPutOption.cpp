#include "AmericanPutOption.h"
#include <algorithm>

AmericanPutOption::AmericanPutOption(double expiry, double strike)
	: AmericanOption(expiry, strike){}

double AmericanPutOption::payoff(double spot) const {
	double strike = getStrike();
    
    if (strike > spot) {
        return strike - spot;
    } else {
        return 0.0;
    }
}

AmericanPutOption::~AmericanPutOption() = default;
