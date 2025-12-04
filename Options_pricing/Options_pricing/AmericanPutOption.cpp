#include "AmericanPutOption.h"


#include <algorithm>



AmericanPutOption::AmericanPutOption(double expiry, double strike)


	: AmericanOption(expiry, strike)


{


}



double AmericanPutOption::payoff(double spot) const {


	// Payoff Put : max(K - S, 0)


	return std::max(getStrike() - spot, 0.0);


}



AmericanPutOption::~AmericanPutOption() = default;