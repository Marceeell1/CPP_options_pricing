#pragma once
#include "Option.h"

class EuropeanVanillaOption : public Option
{
public:
    enum class optionType { call, put };

    EuropeanVanillaOption(double expiry, double strike);

    double getStrike() const;

    // pure virtual method
    virtual optionType GetOptionType() const = 0;

    // virtual destructor
    virtual ~EuropeanVanillaOption() = default;

    friend class BlackScholesPricer;

private:
    double _strike;
};


