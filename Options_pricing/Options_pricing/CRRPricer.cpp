#include "CRRPricer.h"
#include "Option.h"  
#include <cmath>      


//constructor
CRRPricer::CRRPricer(Option* option, int depth,
    double asset_price, double up, double down, double interest_rate)
    : _option(option),
    _N(depth),
    _S0(asset_price),
    _U(up), _D(down), _R(interest_rate),
    _q(0.0),
    _computed(false),
    _H() 
{
    // Basic validations
    if (!_option) throw std::invalid_argument("CRRPricer: option is null");
    if (_N < 0)   throw std::invalid_argument("CRRPricer: depth < 0");
    if (_S0 <= 0) throw std::invalid_argument("CRRPricer: S0 must be > 0");

    //No-arbitrage: D < R < U
    if (!(_D < _R && _R < _U))
        throw std::invalid_argument("CRRPricer: require D < R < U");

    // Risk-neutral probability
    _q = (_R - _D) / (_U - _D);
    if (_q < 0.0 || _q > 1.0)
        throw std::invalid_argument("CRRPricer: q not in [0,1]");


    _H.setDepth(_N);
}

// S(n,i) = S0 * (1+U)^i * (1+D)^(n-i)
double CRRPricer::stockAt(int n, int i) const {

    return _S0 * std::pow(1.0 + _U, i) * std::pow(1.0 + _D, n - i);
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

	// Price at maturity
    for (int i = 0; i <= _N; ++i) {
        double SNi = stockAt(_N, i);
        _H.setNode(_N, i, _option->payoff(SNi));
    }

    // Backward step
    const double disc = 1.0 / (1.0 + _R);
    for (int n = _N - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            double upVal = _H.getNode(n + 1, i + 1);
            double downVal = _H.getNode(n + 1, i);
            double hn = disc * (_q * upVal + (1.0 - _q) * downVal);
            _H.setNode(n, i, hn);
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




double CRRPricer::operator()(bool closed_form) {
    // Default: use CRR procedure
    if (!closed_form) {
        if (!_computed) compute();
        return _H.getNode(0, 0);
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
