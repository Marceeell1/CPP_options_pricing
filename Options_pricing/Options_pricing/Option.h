#pragma once

class Option
{
public:
    // Constructor
    Option(double expiry);

    // Getter for expiry
    double getExpiry() const;

    // Pure virtual payoff function
    virtual double payoff(double spot) const = 0;

    // Virtual destructor
    virtual ~Option();

private:
    double _expiry;
};
