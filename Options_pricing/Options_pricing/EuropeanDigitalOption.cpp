#include "EuropeanDigitalOption.h"
#include <stdexcept>


// Constructor 
EuropeanDigitalOption::EuropeanDigitalOption(double expiry, double strike)
    : Option(expiry), _strike(strike)
{
    if (expiry < 0.0 || strike < 0.0)
        throw std::invalid_argument("Expiry and strike must be non-negative.");
}

//to get the strike
double EuropeanDigitalOption::getStrike() const
{
    return _strike;
}
