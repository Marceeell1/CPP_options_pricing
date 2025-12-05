#ifndef BLACKSCHOLESMCPRICER_H
#define BLACKSCHOLESMCPRICER_H
#pragma once
#include "Option.h"
#include "MT.h"
#include <vector>
class BlackScholesMCPricer
{
public:
	BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility);
	
	int getNbPaths() const;
	
	void generate(int nb_paths);

	std::vector<double> generatePath();

	double operator()() const;

	std::vector<double> confidenceInterval() const;

private:
	Option* _option;
	double _S0;
	double _r;
	double _sigma;

	int _nbPaths;
	double _mean;
	double _M2;
};
#endif
