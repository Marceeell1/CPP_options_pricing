#pragma once
#include "EuropeanDigitalOption.h"

class EuropeanDigitalPutOption : public EuropeanDigitalOption
{
public:
    EuropeanDigitalPutOption(double expiry, double strike)
        : EuropeanDigitalOption(expiry, strike) {
    }
    double payoff(double spot) const override
    {
        return (spot >= getStrike()) ? 1.0 : 0.0;
    }
    optionType GetOptionType() const override
    {
        return optionType::put;
    }
};
