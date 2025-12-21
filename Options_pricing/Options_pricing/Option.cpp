#include "Option.h"
#include <iostream>
#include <stdexcept>

// Constructor
Option::Option(double expiry) : _expiry(expiry) {}

// Getter
double Option::getExpiry() const {
    return _expiry;
}

double Option::payoffPath(const std::vector<double>& path) const {
    if (path.empty()) {
        throw std::invalid_argument("Option::payoffPath: empty path");
    }
    return payoff(path.back());
}
// Virtual destructor
Option::~Option() = default;

