#include <iostream>
#include "CallOption.h"
#include "PutOption.h"
#include "BlackScholesPricer.h"

void TestPart1()
{
    try
    {
        // Parameters
        double expiry = 1.0;       // 1 year
        double strike = 100.0;     // K
		double S = 105.0;          // price of the underlying asset
		double r = 0.05;           // interest rate (5%)
        double sigma = 0.2;        // volatility (20%)

		// Options creation
        CallOption call(expiry, strike);
        PutOption put(expiry, strike);

		// Pricers creation
        BlackScholesPricer call_pricer(&call, S, r, sigma);
        BlackScholesPricer put_pricer(&put, S, r, sigma);

		// Computing prices and deltas
        double call_price = call_pricer();
        double put_price = put_pricer();
        double call_delta = call_pricer.delta();
        double put_delta = put_pricer.delta();

		// Output results
        std::cout << "> Black-Scholes Test\n\n";
        std::cout << "Underlying Price (S): " << S << "\n";
        std::cout << "Strike (K):           " << strike << "\n";
        std::cout << "Expiry (T):           " << expiry << " year\n";
        std::cout << "Interest Rate (r):    " << r << "\n";
        std::cout << "Volatility (sigma):   " << sigma << "\n\n";

        std::cout << "Call Option Price:  " << call_price << "\n";
        std::cout << "Put Option Price:   " << put_price << "\n";
        std::cout << "Call Delta:         " << call_delta << "\n";
        std::cout << "Put Delta:          " << put_delta << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error : " << e.what() << std::endl;
    }
}

int main()
{
	TestPart1();
    return 0;
}