#include "myFunction.hpp"

double eval(double x) { return (x >= 0.) ? x : -x; }

void printData(double data, double dataError, double value, double valueError,
               std::string u) {
  if ((value + valueError >= data - dataError &&
       value - valueError <= data + dataError) ||
      (value + valueError <= data - dataError &&
       value - valueError >= data + dataError)) {
    std::cout << "\033[32m" << data << " ± " << dataError << ' ' << u << "\033[0m";
  } else {
    std::cout << "\033[35m" << data << " ± " << dataError << ' ' << u
              << "   dis.: " << (value - data) / dataError
              << " error\033[0m";
  }
}

void printData(double data, double dataError, double value, std::string u) {
  if (value >= data - dataError && value <= data + dataError) {
    std::cout << "\033[32m" << data << " ± " << dataError << ' ' << u
              << "\033[0m";
  } else {
    std::cout << "\033[35m" << data << " ± " << dataError << ' ' << u
              << "   dis.: " << (value - data) / dataError
              << " error\033[0m";
  }
}

void printData(double data, double dataError, std::string u) {
  std::cout << data << " ± " << dataError << ' ' << u;
}

void Statistic(double& X, double& Y, double& XStd, double& Ystd) {
  std::ifstream in;

  double x{}, y{};
  double dx2{}, dy2{};
  int N{};

  X = 0.;
  Y = 0.;
  XStd = 0.;
  Ystd = 0.;

  // Calcolo media e deviazione standard sulle x e sulle y
  in.open("rumore.txt");

  while (true) {
    in >> x >> y;
    if (!in.good()) {
      break;
    } else {
      ++N;
      X += x;
      Y += y;
    }
  }

  in.close();

  // medie
  X = X / N;
  Y = Y / N;

  in.open("rumore.txt");

  while (true) {
    in >> x >> y;
    if (!in.good()) {
      break;
    } else {
      ++N;
      dx2 += pow(x - X, 2);
      dy2 += pow(y - Y, 2);
    }
  }

  in.close();

  // deviazioni standard
  XStd = sqrt(dx2 / (N - 1));
  Ystd = sqrt(dy2 / (N - 1));
}