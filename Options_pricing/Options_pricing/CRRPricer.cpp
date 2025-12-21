#include "CRRPricer.h"
#include "Option.h"  
#include <cmath>      
#include <algorithm>  
#include <stdexcept> 

// Common initialization function
void CRRPricer::initializePricer(Option* option, int depth, double asset_price, double U, double D, double R)
{
    _option = option;
    _N = depth;
    _S0 = asset_price;
    _U = U;
    _D = D;
    _R = R;
    _computed = false;
    _q = 0.0;

    // Basic validations
    if (!_option) throw std::invalid_argument("CRRPricer: option is null");

    if (_option->isAsianOption())
        throw std::invalid_argument("CRRPricer: cannot price Asian options with CRR");

    if (_N < 0)   throw std::invalid_argument("CRRPricer: depth < 0");
    if (_S0 <= 0) throw std::invalid_argument("CRRPricer: S0 must be > 0");

    // No-arbitrage: D < R < U
    if (!(_D < _R && _R < _U))
        throw std::invalid_argument("CRRPricer: require D < R < U (no arbitrage)");

    // Risk-neutral probability q = (R - D) / (U - D)
    _q = (_R - _D) / (_U - _D);

    if (_q <= 0.0 || _q >= 1.0)
        throw std::invalid_argument("CRRPricer: q not strictly in (0,1)");

    // Initialize trees
    _H.setDepth(_N);
    // Initialize exercise tree only if needed
    if (_option->isAmericanOption()) {
        _exercisePolicy.setDepth(_N);
    }
}
// Constructor 1 (standard CRR)
CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate) :
    _H(), _exercisePolicy()
{
    initializePricer(option, depth, asset_price, up, down, interest_rate);
}

// Constructor 2 (Surcharge)
CRRPricer::CRRPricer(Option* option, int depth,
    double asset_price, double r, double volatility)
    : _H(), _exercisePolicy(),
    _N(depth)
{
    if (_N <= 0) throw std::invalid_argument("CRRPricer (BS approx): depth must be > 0");

    double T = option->getExpiry();
    double h = T / depth;

    double drift = (r + 0.5 * volatility * volatility) * h;

    double U_bs = std::exp(drift + volatility * std::sqrt(h)) - 1.0;
    double D_bs = std::exp(drift - volatility * std::sqrt(h)) - 1.0;
    double R_bs = std::exp(r * h) - 1.0;


    initializePricer(option, depth, asset_price, U_bs, D_bs, R_bs);

}


//here, we use these helper functions to speed up the computation power and to avoid repetitive calculation
void fastPowStep(double& current, double base)
{
    current *= base;
}

void binomCoeffStep(double& current, int N, int i)
{
    // C(N,i+1) = C(N,i) * (N - i) / (i + 1)
    current *= static_cast<double>(N - i) / static_cast<double>(i + 1);
}



// S(n,i) = S0*(1+U)^i * (1+D)^(n-i)
double CRRPricer::stockAt(int n, int i) const
{
    double prod = 1.0;

    for (int k = 0; k < n; ++k) {
        if (k < i) fastPowStep(prod, 1.0 + _U);
        else       fastPowStep(prod, 1.0 + _D);
    }

    return _S0 * prod;
}





// Backward induction: CRR
void CRRPricer::compute() {

    //Forward step: compute payoffs at maturity
    for (int i = 0; i <= _N; ++i) {
        double SNi = stockAt(_N, i);
        double payoffVal = _option->payoff(SNi);
        _H.setNode(_N, i, payoffVal);

        if (_option->isAmericanOption()) {
            _exercisePolicy.setNode(_N, i, true);
        }
    }
    //Backward step
    const double disc = 1.0 / (1.0 + _R);
    for (int n = _N - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {


            double upVal = _H.getNode(n + 1, i + 1);
            double downVal = _H.getNode(n + 1, i);
            double continuationValue = disc * (_q * upVal + (1.0 - _q) * downVal);

            double finalValue;

            if (_option->isAmericanOption()) {
                double intrinsicValue = _option->payoff(stockAt(n, i));

                finalValue = std::max(continuationValue, intrinsicValue);

                bool exercise = (intrinsicValue >= continuationValue);
                _exercisePolicy.setNode(n, i, exercise);

            }
            else {
                finalValue = continuationValue;
            }
            _H.setNode(n, i, finalValue);
        }
    }
    _computed = true;
}
// to get the value of H(n,i) 
double CRRPricer::get(int n, int i) const {
    if (!_computed)
        throw std::logic_error("CRRPricer::get: compute() not called yet");
    return _H.getNode(n, i);
}


// to get the exercise policy at node (n,i) for American options
bool CRRPricer::getExercise(int n, int i) const {
    if (!_computed)
        throw std::logic_error("CRRPricer::getExercise: compute() not called yet");
    if (!_option->isAmericanOption())
        throw std::logic_error("CRRPricer::getExercise: option is not American, no policy stored");

    return _exercisePolicy.getNode(n, i);
}


// Return price H(0,0)
double CRRPricer::operator()(bool closed_form) {
    // Default: use CRR procedure
    if (!closed_form) {
        if (!_computed) compute();
        return _H.getNode(0, 0);
    }

    if (_option->isAmericanOption() && closed_form) {
        throw std::logic_error("CRRPricer::operator(): Closed-form formula is not applicable to American options.");
    }

    // Closed-form
    double sum = 0.0;

    // Initialisations for i = 0
    double coeff = 1.0;            // C(N,0)
    double qPow = 1.0;             // q^0
    double oneMinusQPow = 1.0;     // (1-q)^N

    // (1-q)^N
    for (int k = 0; k < _N; ++k)
        fastPowStep(oneMinusQPow, 1.0 - _q);

    const double invOneMinusQ = 1.0 / (1.0 - _q);

    for (int i = 0; i <= _N; ++i) {

        double SNi = stockAt(_N, i);
        double hval = _option->payoff(SNi);

        sum += coeff * qPow * oneMinusQPow * hval;

        // update when we go from i to i+1
        if (i < _N) {
            binomCoeffStep(coeff, _N, i);      // C(N,i+1)
            fastPowStep(qPow, _q);             // q^(i+1)
            fastPowStep(oneMinusQPow, invOneMinusQ); // (1-q)^(N-(i+1))
        }
    }

    double disc = 1.0;
    for (int k = 0; k < _N; ++k)
        fastPowStep(disc, 1.0 + _R);

    return sum / disc;

}