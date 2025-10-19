#include "BlackScholesPricer.h"
#include <cmath>

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option,
    double asset_price,
    double interest_rate,
    double volatility)
    : _option(option), _S(asset_price), _r(interest_rate), _sigma(volatility)
{
}

// Normal cumulative distribution function
double BlackScholesPricer::N(double x) const
{
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

// Price of the option
double BlackScholesPricer::operator()() const
{
    double T = _option->getExpiry();
    double K = _option->getStrike();

    double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));
    double d2 = d1 - _sigma * std::sqrt(T);

    if (_option->GetOptionType() == EuropeanVanillaOption::optionType::call)
    {
        return _S * N(d1) - K * std::exp(-_r * T) * N(d2);
    }
    else // put
    {
        return K * std::exp(-_r * T) * N(-d2) - _S * N(-d1);
    }
}

// Delta
double BlackScholesPricer::delta() const
{
    double T = _option->getExpiry();
    double K = _option->getStrike();

    double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));

    if (_option->GetOptionType() == EuropeanVanillaOption::optionType::call)
        return N(d1);
    else
        return N(d1) - 1;
}
