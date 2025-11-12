#include <iostream>
#include "CallOption.h"
#include "PutOption.h"
#include "BlackScholesPricer.h"
#include "CRRPricer.h"
#include <cmath>


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

class EuropeanCall : public Option {
public:
    EuropeanCall(double expiry, double strike)
        : Option(expiry), _K(strike) {
    }
    double payoff(double S) const override {
        return (S > _K) ? (S - _K) : 0.0;
    }
private:
    double _K;
};


void TestPart2() {
    std::cout << "=== Test CRR Model (N=3) ===" << std::endl;

    // 1) Parameters
    double S0 = 100.0;   // initial stock price
    double U = 0.1;     // up move +10%
    double D = -0.1;    // down move -10%
    double R = 0.02;    // risk-free rate +2%
    int N = 3;       // number of steps
    double K = 100.0;   // strike price

    // 2) Create a simple option (Call)
    EuropeanCall call(1.0, K);

    // 3) Create CRR pricer
    CRRPricer pricer(&call, N, S0, U, D, R);

    // 4) Compute option price
    double price_tree = pricer();             // standard CRR method
    double price_cf = pricer(true);         // closed-form

    // 5) Display results
    std::cout << "Price by CRR tree  : " << price_tree << std::endl;
    std::cout << "Price by closed form: " << price_cf << std::endl;

    // 6) Display the internal H tree (option values)
    std::cout << "\n=== Tree of H(n,i) ===" << std::endl;
    pricer.compute();         // make sure it's computed
    // unfortunately BinaryTree is private, so we can't access it directly
    // ? but we can re-display the stock prices for demonstration instead

    BinaryTree<double> tree(N);
    for (int n = 0; n <= N; ++n) {
        for (int i = 0; i <= n; ++i) {
            double Sni = S0 * std::pow(1.0 + U, i) * std::pow(1.0 + D, n - i);
            tree.setNode(n, i, Sni);
        }
    }

    tree.display();  // show the stock price tree

    std::cout << "\n=== End of Test ===" << std::endl;
}

// ---- main ----
int main() {
    TestPart2();
    return 0;
}