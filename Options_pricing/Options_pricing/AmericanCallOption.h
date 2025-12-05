#pragma once
#include "AmericanOption.h"

class AmericanCallOption : public AmericanOption
{
public:
    AmericanCallOption(double expiry, double strike);

    double payoff(double spot) const override;

    virtual ~AmericanCallOption();
};