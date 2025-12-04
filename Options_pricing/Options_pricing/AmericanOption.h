#pragma once

#include "Option.h"


class AmericanOption : public Option

{

public:

    // Constructor

    AmericanOption(double expiry, double strike);


    // Getter for strike

    double getStrike() const;


    // Override to return true

    bool isAmericanOption() const override;


    // Destructor

    virtual ~AmericanOption();


private:

    double _strike;

};