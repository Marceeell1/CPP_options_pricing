#include "BlackScholesMCPricer.h"
#include <cmath>
#include <stdexcept>

BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility)
	:_option(option),_S0(initial_price),_r(interest_rate),_sigma(volatility),_nbPaths(0),_mean(0.0),_M2(0.0)
{
    if (!_option) throw std::invalid_argument("Option is null");
    if (_S0 <= 0.0) throw std::invalid_argument("Initial price must be positive");
    if (_sigma < 0.0) throw std::invalid_argument("Volatility must be non-negative");
}

int BlackScholesMCPricer::getNbPaths() const { return _nbPaths; }

std::vector<double> BlackScholesMCPricer::generatePath(int m) {
    std::vector<double> path(m);
    path[0] = _S0;  // initial price    
    
    double dt = 1.0 / m;

    for (int i = 1; i < m; ++i) {
        double dz = MT::rand_norm();
        path[i] = path[i - 1] * exp((_r - 0.5 * _sigma * _sigma)*dt + (_sigma * dz)*sqrt(dt));
    }

    return path;
}



void BlackScholesMCPricer::generate(int nb_paths) {
    if (nb_paths <= 0) throw std::invalid_argument("number of paths must be > 0");

    double sum_payoff = 0.0;
    double sum_square_payoff = 0.0;

    // Simulate paths
    for (int i = 0; i < nb_paths; ++i) {
        std::vector<double> path = generatePath(100);
        double payoff = _option->payoffPath(path);
        sum_payoff += payoff;
        sum_square_payoff += payoff * payoff;
    }

    _nbPaths = nb_paths;
    _mean = sum_payoff / nb_paths;
    _M2 = sum_square_payoff / nb_paths - _mean * _mean;  // Calculus of the second moment
}

std::vector<double> BlackScholesMCPricer::confidenceInterval() const {
    double variance = _M2;
    double stddev = sqrt(variance);

    std::vector<double> ci(2);
    ci[0] = _mean - 1.96 * stddev / sqrt(_nbPaths);  // inf
    ci[1] = _mean + 1.96 * stddev / sqrt(_nbPaths);  // sup

    return ci;
}
double BlackScholesMCPricer::operator()() const {
    if (_nbPaths == 0) {
        throw std::runtime_error("No simulation");
    }
    return exp(-_r * _option->getExpiry()) * _mean;  // Estimation of the actualised price
}