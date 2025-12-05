#pragma once
#include "Option.h"

class EuropeanDigitalOption : public Option
{
public:
    // Type d'option digitale
    enum class optionType { call, put };

    // Constructeur : initialise expiry (via Option) et strike
    EuropeanDigitalOption(double expiry, double strike);

    // Getter pour le strike
    double getStrike() const;

    // Méthode virtuelle pour connaître le type (call/put)
    virtual optionType GetOptionType() const = 0;

    // Destructeur virtuel
    virtual ~EuropeanDigitalOption() = default;

    // BlackScholesPricer a accès aux membres privés
    friend class BlackScholesPricer;

private:
    double _strike;
};
