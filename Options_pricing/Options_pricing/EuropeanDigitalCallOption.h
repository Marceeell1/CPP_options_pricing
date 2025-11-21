#pragma once
#include "EuropeanDigitalOption.h"

class EuropeanDigitalCallOption : public EuropeanDigitalOption
{
public:
    EuropeanDigitalCallOption(double expiry, double strike)
        : EuropeanDigitalOption(expiry, strike) {
    }

    optionType GetOptionType() const override
    {
        return optionType::call;
    }
};
    