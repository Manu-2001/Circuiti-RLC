#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "TF1.h"

int R(TF1 *const &f, std::string const &fileName) {
  double Y{};
  double x{};
  double y{};
  double dY2{};

  int N{};

  std::ifstream sorgente;

  sorgente.open(fileName);

  // calcola media e scarti (yi - f(xi))^2
  if (!sorgente.is_open()) {
    return EXIT_FAILURE;
  }

  while (true) {
    sorgente >> x >> y;
    if (!sorgente.good()) {
      break;
    } else {
      ++N;
      Y += y;
      dY2 += pow(y - f->Eval(x), 2);
    }
  }

  double const Ym = Y / N;
  double const Numeratore = dY2;
  dY2 = 0.;

  sorgente.close();

  sorgente.open(fileName);
  
  // calcola scarti (y1 -<Y>)^2
  while (true) {
    sorgente >> x >> y;
    if (!sorgente.good()) {
      break;
    } else {
      dY2 += pow(y - Ym, 2);
    }
  }

  double const Denominatore = dY2;

  sorgente.close();

  return 1 - Numeratore / Denominatore;
}