#include "AmericanOption.h"

AmericanOption::AmericanOption(double expiry, double strike)

    : Option(expiry), _strike(strike) {}

double AmericanOption::getStrike() const {
    return _strike;
}

bool AmericanOption::isAmericanOption() const { return true; }

AmericanOption::~AmericanOption() = default;