#pragma once
#include <vector>

class Option
{
public:
    Option(double expiry);

    double getExpiry() const;

    virtual double payoff(double spot) const = 0;

    virtual double payoffPath(const std::vector<double>& path) const;

    virtual bool isAsianOption() const { return false; }

    virtual ~Option();

private:
    double _expiry;
};
