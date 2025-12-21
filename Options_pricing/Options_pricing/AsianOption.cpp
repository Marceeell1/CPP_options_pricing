#include "AsianOption.h"
#include <iostream>
AsianOption::AsianOption(double expiry, const std::vector<double>& timeSteps)
	:Option(expiry),_timeSteps(timeSteps){}
std::vector<double> AsianOption::getTimeSteps() const{
	return _timeSteps;
}
double AsianOption::payoffPath(const std::vector<double>& path) const {
	if (path.empty()) {
		std::cerr << "[Error] payoffPath() : 'path' vector is empty." << std::endl;
		return 0;
	}
	double sum = 0;
	for (double s : path) {
		sum += s;
	}
	double average = sum / static_cast<double>(path.size());
	return payoff(average);

}
AsianOption::AsianOption(const std::vector<double>& timeSteps)
	: Option(timeSteps.back()), _timeSteps(timeSteps) {
}