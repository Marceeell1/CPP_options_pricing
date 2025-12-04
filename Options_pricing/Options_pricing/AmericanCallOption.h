#pragma once
#include "AmericanOption.h"

class AmericanCallOption : public AmericanOption
{
public:
    // Constructeur : appelle celui d'AmericanOption
    AmericanCallOption(double expiry, double strike);

    // Surcharge de la méthode payoff (S - K)+
    double payoff(double spot) const override;

    virtual ~AmericanCallOption();
};