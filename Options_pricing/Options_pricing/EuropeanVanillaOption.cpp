#include "EuropeanVanillaOption.h"
#include <stdexcept>  // for std::invalid_argument

// Constructor with input validation
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike)
    : Option(expiry), _strike(strike)
{
    if (expiry < 0.0 || strike < 0.0)
        throw std::invalid_argument("Expiry and strike must be non-negative.");
}

// Getter for strike
double EuropeanVanillaOption::getStrike() const
{
    return _strike;
}
