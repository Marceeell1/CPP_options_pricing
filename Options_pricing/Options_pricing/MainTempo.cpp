#include "BlackScholesPricer.h"
#include "CRRPricer.h"
#include "AmericanPutOption.h"
#include <cmath>

#include <iostream>
#include <vector>
#include "CallOption.h"
#include "PutOption.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"
#include "AsianCallOption.h"
#include "AsianPutOption.h"
#include "BlackScholesMCPricer.h"


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

    std::cout << "\n=======================================================" << std::endl;
    std::cout << "=== NOUVEAU TEST 1 : American Put Option (N=3) ===" << std::endl;

    // Utilisez des paramètres qui encouragent l'exercice anticipé (Put ITM)
    double K_am = 110.0;
    int T = 1;
    AmericanPutOption am_put(T, K_am);

    // Recréez le pricer (mêmes U, D, R)
    CRRPricer am_pricer(&am_put, N, S0, U, D, R);

    // Exécutez le calcul pour remplir _H et _exercisePolicy
    am_pricer.compute();
    double am_price = am_pricer(); // Prix à H(0,0)

    std::cout << "Option: American Put (K=" << K_am << ")" << std::endl;
    std::cout << "Prix par CRR (Américain) : " << am_price << std::endl;

    // Vérification de la politique d'exercice anticipé (exemple)
    std::cout << "\nTest Politique d'Exercice (getExercise):" << std::endl;
    // On peut vérifier le noeud (n=2, i=0). S(2,0) = 100 * (0.9)^2 = 81
    // Un Put (K=110) est fortement in-the-money ici.
    int n_check = 2;
    int i_check = 0;

    double S_check = S0 * std::pow(1.0 + U, i_check) * std::pow(1.0 + D, n_check - i_check);
    bool should_exercise = am_pricer.getExercise(n_check, i_check);

    std::cout << "Au noeud n=" << n_check << ", i=" << i_check << " (Stock Price S=" << S_check << "):" << std::endl;
    std::cout << "  -> Doit-on exercer ? : " << (should_exercise ? "Oui" : "Non") << std::endl;

    // Note: Pour un Put ITM profond comme S=81, l'exercice anticipé est très probable.

    // --- NOUVEAU TEST 2 : Approximation Black-Scholes (Partie IV, Q5) ---
    std::cout << "\n=======================================================" << std::endl;
    std::cout << "=== NOUVEAU TEST 2 : BS Approximation CRR ===" << std::endl;

    // 1) Paramètres Black-Scholes
    double r_bs = 0.05;       // Taux continu sans risque
    double sigma_bs = 0.20;   // Volatilité
    int N_bs = 500;           // Grand nombre de pas pour l'approximation

    // Utilisez la même option européenne pour la comparaison
    // EuropeanCall call(T, K);

    // 2) Créer CRR pricer avec la surcharge Black-Scholes
    CRRPricer bs_pricer(&call, N_bs, S0, r_bs, sigma_bs);

    // 3) Récupérer les paramètres calculés (Vérification interne du constructeur)
    // Malheureusement, U, D, R sont privés. Si vous aviez un accesseur:
    // std::cout << "  U calculé : " << bs_pricer.getU() << std::endl;
    // std::cout << "  D calculé : " << bs_pricer.getD() << std::endl;
    // std::cout << "  R calculé : " << bs_pricer.getR() << std::endl;

    // 4) Calculer le prix
    double price_bs_approx = bs_pricer();

    std::cout << "Option: European Call (K=" << K << ", T=" << T << ", r=" << r_bs << ", sigma=" << sigma_bs << ")" << std::endl;
    std::cout << "N = " << N_bs << " (Approximation)" << std::endl;
    std::cout << "Prix par Approximation BS : " << price_bs_approx << std::endl;

    // (Idéalement, on comparerait ce prix avec le BlackScholesPricer pour validation)

    std::cout << "\n=== End of All Tests ===" << std::endl;
}

// ---- main ----
int main() {
    TestPart2();
    return 0;
}