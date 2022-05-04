#ifndef MYFUNCTION_HPP
#define MYFUNCTION_HPP

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

// funzione valore assoluto
double eval(double);
// per stampare delle misure e confrontarle con la misura attesa
void printData(double, double, double, double, std::string const&);
void printData(double, double, double, std::string const&);
void printData(double, double, std::string const&);
// calcola media e deviazione standard
void Statistic(double&, double&, double&, double&, std::string const&);

#endif