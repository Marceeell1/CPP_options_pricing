#pragma once

#include "AmericanOption.h"

class AmericanPutOption : public AmericanOption

{

public:

    // Constructeur

    AmericanPutOption(double expiry, double strike);


    // Surcharge of the payoff method (K - S)+

    double payoff(double spot) const override;


    virtual ~AmericanPutOption();

};