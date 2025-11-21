#pragma once
#include "Option.h"
#include <vector>
class AsianOption : public Option
{
protected:
	std::vector<double> _timeSteps;
public:
	AsianOption(double expiry,const std::vector<double>& timeSteps);
	
	std::vector<double> getTimeSteps() const;

	double payoffPath(const std::vector<double>& path) const;

	bool isAsianOption() const override { return true;}
};

