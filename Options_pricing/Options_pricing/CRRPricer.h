#ifndef CRRPRICER_H
#define CRRPRICER_H

#include "BinaryTree.h"
#include <stdexcept>

// declaration only
class Option;

class CRRPricer {
public:
    // Constructor
    CRRPricer(Option* option, int depth,
        double asset_price, double up, double down, double interest_rate);

    CRRPricer(Option* option, int depth,
        double asset_price, double r, double volatility);
    // Compute H(n,i) by backward induction
    void compute();

    bool getExercise(int n, int i) const;
    // Get H(n,i)
    double get(int n, int i) const;

    // Return price H(0,0)
    double operator()(bool closed_form = false);

private:
   
    double stockAt(int n, int i) const;     // S(n,i)
    void initializePricer(Option* option, int depth, double asset_price, double U, double D, double R);


private:
    Option* _option;        
    int     _N;               
    double  _S0;              
    double  _U, _D, _R;        
    double  _q;                
    bool    _computed;          
    BinaryTree<double> _H;      
    BinaryTree<bool> _exercisePolicy;
};
#endif 