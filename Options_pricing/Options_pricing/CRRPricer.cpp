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
CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate): 
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

    double u = std::exp(volatility * std::sqrt(h));
    double d = std::exp(-volatility * std::sqrt(h));
    double Rfac = std::exp(r * h);

    double U_bs = u - 1.0;
    double D_bs = d - 1.0;
    double R_bs = Rfac - 1.0;

    initializePricer(option, depth, asset_price, U_bs, D_bs, R_bs);

}


// here we use our own fast power function since n is integer
double fastPow(double x, int n)
{
    double res = 1.0;
    for (int k = 0; k < n; ++k)
        res *= x;
    return res;
}



// S(n,i) = S0*(1+U)^i * (1+D)^(n-i)
double CRRPricer::stockAt(int n, int i) const
{
    return _S0
        * fastPow(1.0 + _U, i)
        * fastPow(1.0 + _D, n - i);
}


//C(n,k) : binomial coefficient
double CRRPricer::binomCoeff(int n, int k) const {
    if (k < 0 || k > n) return 0.0;
    if (k > n - k) k = n - k;  // symmetry to reduce loop
    long double c = 1.0L;
    for (int i = 1; i <= k; ++i) {
        c = c * (n - (k - i));
        c /= i;
    }
    return static_cast<double>(c);
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

    // Closed-form:
    double sum = 0.0;
    for (int i = 0; i <= _N; ++i) {
        double SNi = stockAt(_N, i);
        double hval = _option->payoff(SNi);
        double term = binomCoeff(_N, i) * std::pow(_q, i) * std::pow(1.0 - _q, _N - i) * hval;
        sum += term;
    }
    double disc = std::pow(1.0 + _R, _N);
    return sum / disc;
}