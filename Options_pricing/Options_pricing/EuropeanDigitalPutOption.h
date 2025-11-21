#pragma once
#include "EuropeanDigitalOption.h"

class EuropeanDigitalPutOption : public EuropeanDigitalOption
{
public:
    EuropeanDigitalPutOption(double expiry, double strike)
        : EuropeanDigitalOption(expiry, strike) {
    }

    optionType GetOptionType() const override
    {
        return optionType::put;
    }
};
