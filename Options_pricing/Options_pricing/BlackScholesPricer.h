#pragma once
#include "EuropeanVanillaOption.h"
#include <cmath>

class BlackScholesPricer
{
public:
	BlackScholesPricer(Option* option, double asset_price,
		double interest_rate, double volatility);

	double operator()() const;
    double delta() const; // returns Delta

private:
	Option* _option;
	double _S;  // price of the underlying asset
	double _r;  // interest rate
	double _sigma;  // volatility

	double N(double x) const; // normal cdf
	void compute_d1_d2(double K, double T, double& d1, double& d2) const;
};