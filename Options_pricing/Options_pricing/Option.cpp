#include "Option.h"
#include <iostream>

// Constructor definition
Option::Option(double expiry) : _expiry(expiry) {}

// Getter definition
double Option::getExpiry() const {
    return _expiry;
}

double Option::payoffPath(const std::vector<double>& path) const {
    if (path.empty()) {
        std::cerr << "[Erreur] payoffPath() : le vecteur 'path' est vide." << std::endl;
        return 0;
    }
    return payoff(path.back());
}
// Virtual destructor definition
Option::~Option() = default;

