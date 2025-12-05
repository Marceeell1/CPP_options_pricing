#pragma once
#include "Option.h"

class EuropeanDigitalOption : public Option
{
public:
	// Option type enumeration
    enum class optionType { call, put };

    // Constructor
    EuropeanDigitalOption(double expiry, double strike);

	// to get the strike
    double getStrike() const;

	// function to get option type
    virtual optionType GetOptionType() const = 0;

	// Virtual destructor
    virtual ~EuropeanDigitalOption() = default;

	// Grant access to BlackScholesPricer
    friend class BlackScholesPricer;

private:
    double _strike;
};
