#pragma once
#include "EuropeanVanillaOption.h"
#include <cmath>  // for exp, log, sqrt, erfc

class BlackScholesPricer
{
public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price,
        double interest_rate, double volatility);

	double operator()() const;  // returns the option price
    double delta() const;       // returns Delta

private:
    EuropeanVanillaOption* _option;
	double _S;  // price of the underlying asset
	double _r;  // interest rate
	double _sigma;  // volatility

	double N(double x) const; // normal cumulative distribution function
};
