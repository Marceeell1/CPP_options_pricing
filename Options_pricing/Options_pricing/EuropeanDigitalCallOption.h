#pragma once
#include "EuropeanDigitalOption.h"

class EuropeanDigitalCallOption : public EuropeanDigitalOption
{
public:

	// Constructor
    EuropeanDigitalCallOption(double expiry, double strike)
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
        return optionType::call;
    }
};