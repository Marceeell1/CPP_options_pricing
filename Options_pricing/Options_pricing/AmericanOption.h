#pragma once
#include "Option.h"

class AmericanOption : public Option
{
public:

    AmericanOption(double expiry, double strike);

    double getStrike() const;

    bool isAmericanOption() const override;

    virtual ~AmericanOption();

private:
    double _strike;
};