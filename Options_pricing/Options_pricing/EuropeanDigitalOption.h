#pragma once
#include "Option.h"

class EuropeanDigitalOption : public Option
{
public:
	// Option type enumeration
    enum class optionType { call, put };

    EuropeanDigitalOption(double expiry, double strike);

    double getStrike() const;

    virtual optionType GetOptionType() const = 0;

	// Virtual destructor
    virtual ~EuropeanDigitalOption() = default;

	// Access to BlackScholesPricer
    friend class BlackScholesPricer;

private:
    double _strike;
};
