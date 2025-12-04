#include "AmericanOption.h"
// Constructor definition

AmericanOption::AmericanOption(double expiry, double strike)

    : Option(expiry), _strike(strike) {}


// Getter definition

double AmericanOption::getStrike() const {

    return _strike;

}

bool AmericanOption::isAmericanOption() const { return true; }
// Destructor definition

AmericanOption::~AmericanOption() = default;