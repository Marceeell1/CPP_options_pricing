#include "BlackScholesPricer.h"

#include "Option.h"
#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"

#include <cmath>
#include <stdexcept>
const double PI = 3.14159265358979323846;

BlackScholesPricer::BlackScholesPricer(Option* option,
    double asset_price,
    double interest_rate,
    double volatility)
    : _option(option), _S(asset_price), _r(interest_rate), _sigma(volatility)
{
}

// Normal cdf
double BlackScholesPricer::N(double x) const
{
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

// Price of the option
double BlackScholesPricer::operator()() const
{
    // Check if the option is a European vanilla option
    if (auto* vanilla = dynamic_cast<EuropeanVanillaOption*>(_option))
    {
        double T = vanilla->getExpiry();
        double K = vanilla->getStrike();

        double d1, d2;
        compute_d1_d2(K, T, d1, d2);

        if (vanilla->GetOptionType() == EuropeanVanillaOption::optionType::call)
        {
            return _S * N(d1) - K * std::exp(-_r * T) * N(d2);
        }
        else // put
        {
            return K * std::exp(-_r * T) * N(-d2) - _S * N(-d1);
        }
    }

    // Check if the option is a European digital option
    if (auto* digital = dynamic_cast<EuropeanDigitalOption*>(_option))
    {
        double T = digital->getExpiry();
        double K = digital->getStrike();

        double d1, d2;
        compute_d1_d2(K, T, d1, d2);

        // Cash or nothing
        if (digital->GetOptionType() == EuropeanDigitalOption::optionType::call)
        {
            return std::exp(-_r * T) * N(d2); // digital call price
        }
        else
        {
            return std::exp(-_r * T) * N(-d2); // digital put price
        }
    }

    // Unsupported
    throw std::runtime_error("BlackScholesPricer: unsupported option type");
}

// Delta
double BlackScholesPricer::delta() const
{
    // Delta for European vanilla options
    if (auto* vanilla = dynamic_cast<EuropeanVanillaOption*>(_option))
    {
        double T = vanilla->getExpiry();
        double K = vanilla->getStrike();

        double d1, d2;
        compute_d1_d2(K, T, d1, d2);

        if (vanilla->GetOptionType() == EuropeanVanillaOption::optionType::call)
            return N(d1);
        else
            return N(d1) - 1.0;
    }

    // Delta for European digital options
    if (auto* digital = dynamic_cast<EuropeanDigitalOption*>(_option))
    {
        double T = digital->getExpiry();
        double K = digital->getStrike();

        double d1, d2;
        compute_d1_d2(K, T, d1, d2);

        const double inv_sqrt_2pi = 1.0 / std::sqrt(2.0 * PI);
        double pdf_d2 = inv_sqrt_2pi * std::exp(-0.5 * d2 * d2);

        double common = std::exp(-_r * T) * pdf_d2 / (_S * _sigma * std::sqrt(T));

        if (digital->GetOptionType() == EuropeanDigitalOption::optionType::call)
            return common; // digital call delta
        else
            return -common; // digital put delta
    }

    throw std::runtime_error("BlackScholesPricer: delta not implemented for this option type");
}

// Compute d1 and d2 for the Black Scholes formula
void BlackScholesPricer::compute_d1_d2(double K, double T,
    double& d1, double& d2) const
{
    double sqrtT = std::sqrt(T);
    d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T)
        / (_sigma * sqrtT);
    d2 = d1 - _sigma * sqrtT;
}