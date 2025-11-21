#ifndef CRRPRICER_H
#define CRRPRICER_H

#include "BinaryTree.h"
#include <stdexcept>

// declaration only
class Option;

class CRRPricer {
public:
    // Constructor:
    CRRPricer(Option* option, int depth,
        double asset_price, double up, double down, double interest_rate);

    // Compute H(n,i) by backward induction (CRR procedure)
    void compute();

    // Get H(n,i)
    double get(int n, int i) const;

    // Return price H(0,0).
    // - closed_form = false (default): uses backward induction
    // - closed_form = true           : uses the binomial closed-form
    double operator()(bool closed_form = false);

private:
   
    double stockAt(int n, int i) const;     // S(n,i) = S0*(1+U)^i * (1+D)^(n-i)
    double binomCoeff(int n, int k) const;  // simple C(n,k) iteration (no big factorials)

private:
    Option* _option;        
    int     _N;               
    double  _S0;              
    double  _U, _D, _R;        
    double  _q;                
    bool    _computed;          
    BinaryTree<double> _H;      
};

#endif 
