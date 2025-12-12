#pragma once
#include "AsianOption.h"
#include <vector>
class AsianCallOption : public AsianOption
{
private:
	double _strike;
public:
	AsianCallOption(const std::vector<double>& timeSteps, double strike);
	double payoff(double spot) const override;
};

