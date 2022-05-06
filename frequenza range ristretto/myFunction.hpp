#ifndef MYFUNCTION_HPP
#define MYFUNCTION_HPP

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

// funzione valore assoluto
double eval(double);
// stampa la misura X e confronta con quella atessa Y
//                X      dx       Y      dy      unità di musura
void printData(double, double, double, double, std::string const&);
//                X      dx       Y      unità di musura
void printData(double, double, double, std::string const&);
//                X      dx      unità di musura
void printData(double, double, std::string const&);
// calcola media e deviazione standard e lo segna in X, XStd...
//                X       XStd      Y       YStd   nome file con il rumore
void Statistic(double&, double&, double&, double&, std::string const&);

#endif