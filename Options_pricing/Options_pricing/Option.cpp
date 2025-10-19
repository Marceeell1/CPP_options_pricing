#include "Option.h"

// Constructor definition
Option::Option(double expiry) : _expiry(expiry) {}

// Getter definition
double Option::getExpiry() const {
    return _expiry;
}

// Virtual destructor definition
Option::~Option() = default;

