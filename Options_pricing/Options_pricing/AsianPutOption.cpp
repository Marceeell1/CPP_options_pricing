#include "AsianPutOption.h"
#include <iostream>
AsianPutOption::AsianPutOption(const std::vector<double>& timeSteps, double strike)
	: AsianOption(timeSteps.back(), timeSteps), _strike(strike) {
	if (strike < 0) {
		std::cerr << "[Error] Negative strike in AsianPutOption.\n";
	}
}
double AsianPutOption::payoff(double spot) const {
	double d = _strike - spot;
	if (d > 0) {
		return d;
	}
	else {
		return 0;
	}
}