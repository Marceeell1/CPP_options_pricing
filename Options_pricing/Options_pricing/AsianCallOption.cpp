#include "AsianCallOption.h"
#include <iostream>
AsianCallOption::AsianCallOption(double expiry, double strike, const std::vector<double>& timeSteps)
	:AsianOption(expiry, timeSteps), _strike(strike) {
	if (strike < 0) {
		std::cerr << "[Error] Negative strike in AsianCallOption.\n";
	}
}
double AsianCallOption::payoff(double spot) const {
	double d = spot - _strike;
	if (d > 0) {
		return d;
	}
	else {
		return 0;
	}
}

