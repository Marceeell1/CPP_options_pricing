#pragma once
#include "EuropeanDigitalOption.h"

class EuropeanDigitalCallOption : public EuropeanDigitalOption
{
public:
    EuropeanDigitalCallOption(double expiry, double strike)
        : EuropeanDigitalOption(expiry, strike) {
    }
    double payoff(double spot) const override
    {
        return (spot >= getStrike()) ? 1.0 : 0.0;
    }
    optionType GetOptionType() const override
    {
        return optionType::call;
    }
};