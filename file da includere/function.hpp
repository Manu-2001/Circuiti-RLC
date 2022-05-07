#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "TF1.h"
#include "misura.hpp"

// funzione valore assoluto
double eval(double);
// stampa la misura X e confronta con quella atessa Y
//                X      dx       Y      dy      unità di musura
bool printData(misura const&, misura const&, std::string const&);
//                X      dx       Y      unità di musura
bool printData(misura const&, double, std::string const&);
//                X      dx      unità di musura
void printData(misura const&, std::string const&);
// calcola media e deviazione standard e lo segna in X, XStd...
//                X       XStd      Y       YStd   nome file con il rumore
void Statistic(double&, double&, double&, double&, std::string const&);
// intersezione fra due funzioni paraboliche, il risultato è in X1, X2, Y1, Y2
// si può scegliere se prendere errori in quadratura o no
//                  X1       X2      Y1        Y2          f           g
int Intersezione(misura&, misura&, misura&, misura&, TF1* const&, TF1* const&,
                 bool);

#endif