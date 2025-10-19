#pragma once
#include "Option.h"

class EuropeanVanillaOption : public Option
{
public:
    // Enum for option type
    enum class optionType { call, put };

    // Constructor: initializes _expiry and _strike
    EuropeanVanillaOption(double expiry, double strike);

    // Getter for strike
    double getStrike() const;

    // Pure virtual method
    virtual optionType GetOptionType() const = 0;

    // Virtual destructor
    virtual ~EuropeanVanillaOption() = default;

private:
    double _strike;
};


