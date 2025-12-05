#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"
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

std::vector<double> BlackScholesMCPricer::generatePath()
{
    std::vector<double> path;
    const double T = _option->getExpiry();

    // asian case : we use the timeSteps
    if (_option->isAsianOption()) {
        const AsianOption* asian = dynamic_cast<const AsianOption*>(_option);
        if (!asian) {
            throw std::runtime_error("BlackScholesMCPricer: isAsianOption() is true but dynamic_cast failed");
        }

        const std::vector<double>& times = asian->getTimeSteps();
        if (times.empty())
            throw std::runtime_error("BlackScholesMCPricer: AsianOption has empty timeSteps");

        path.resize(times.size());
        path[0] = _S0;

        for (std::size_t k = 1; k < times.size(); ++k) {
            double dt = times[k] - times[k - 1];
            if (dt <= 0.0)
                throw std::runtime_error("BlackScholesMCPricer: non-positive time step in AsianOption");

            double dz = MT::rand_norm();
            path[k] = path[k - 1] * std::exp(
                (_r - 0.5 * _sigma * _sigma) * dt +
                _sigma * std::sqrt(dt) * dz
            );
        }
    }
    // european case: one point at T
    else {
        path.resize(1);
        double dz = MT::rand_norm();
        path[0] = _S0 * std::exp(
            (_r - 0.5 * _sigma * _sigma) * T +
            _sigma * std::sqrt(T) * dz
        );
    }

    return path;
}



void BlackScholesMCPricer::generate(int nb_paths) {
    if (nb_paths <= 0)
        throw std::invalid_argument("BlackScholesMCPricer::generate: nb_paths must be > 0");

    const double T = _option->getExpiry();

    for (int i = 0; i < nb_paths; ++i) {
        std::vector<double> path = generatePath();

        // 2. Calculate the payoff for the generated path
        double payoff = _option->payoffPath(path);

        // 3. Discount the payoff to time 0 
        double discPayoff = std::exp(-_r * T) * payoff;


        _nbPaths++;// Increase the path count


        // Welford's algorithm for updating the mean and variance:
        double delta = discPayoff - _mean;
        _mean += delta / static_cast<double>(_nbPaths);
        double delta2 = discPayoff - _mean;
        _M2 += delta * delta2;
    }
}

std::vector<double> BlackScholesMCPricer::confidenceInterval() const {
    if (_nbPaths < 2)
        throw std::runtime_error("BlackScholesMCPricer::confidenceInterval: not enough paths");

    double variance = _M2 / static_cast<double>(_nbPaths - 1);  // unbiased variance
    double stddev = std::sqrt(variance);

    std::vector<double> ci(2);
    ci[0] = _mean - 1.96 * stddev / std::sqrt(static_cast<double>(_nbPaths));
    ci[1] = _mean + 1.96 * stddev / std::sqrt(static_cast<double>(_nbPaths));

    return ci;
}

double BlackScholesMCPricer::operator()() const {
    if (_nbPaths == 0) {
        throw std::runtime_error("No simulation");
    }
    return exp(-_r * _option->getExpiry()) * _mean;  // Estimation of the actualised price
}