#pragma once
#include "EuropeanDigitalOption.h"

// European Digital Put Option class
class EuropeanDigitalPutOption : public EuropeanDigitalOption
{
public:

	// Constructor
    EuropeanDigitalPutOption(double expiry, double strike)
        : EuropeanDigitalOption(expiry, strike) {
    }

	// Payoff implementation
    double payoff(double spot) const override
    {
        return (spot >= getStrike()) ? 1.0 : 0.0;
    }

	// Get option type
    optionType GetOptionType() const override
    {
        return optionType::put;
    }
};
