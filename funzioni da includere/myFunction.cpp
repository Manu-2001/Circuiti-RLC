#include "myFunction.hpp"

double eval(double x) { return (x > 0.) ? x : -x; }

void printData(double X, double dx, double Y, double dy, std::string const& u) {
  if ((Y + dy >= X - dx && Y - dy <= X + dx) ||
      (Y + dy <= X - dx && Y - dy >= X + dx)) {
    std::cout << "\033[32m" << X << " ± " << dx << ' ' << u << "\033[0m";
  } else {
    std::cout << "\033[35m" << X << " ± " << dx << ' ' << u
              << "   dis.: " << eval(Y - X) / dx << " error\033[0m";
  }
}

void printData(double X, double dx, double Y, std::string const& u) {
  if (Y >= X - dx && Y <= X + dx) {
    std::cout << "\033[32m" << X << " ± " << dx << ' ' << u << "\033[0m";
  } else {
    std::cout << "\033[35m" << X << " ± " << dx << ' ' << u
              << "   dis.: " << eval(Y - X) / dx << " error\033[0m";
  }
}

void printData(double X, double dx, std::string const& u) {
  std::cout << X << " ± " << dx << ' ' << u;
}

void Statistic(double& X, double& Y, double& XStd, double& YStd,
               std::string const& fileName) {
  std::ifstream in;

  double x{}, y{};
  double dx2{}, dy2{};
  int N{};

  X = 0.;
  Y = 0.;
  XStd = 0.;
  YStd = 0.;

  // Calcolo media e deviazione standard sulle x e sulle y
  in.open(fileName);

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

  in.open(fileName);

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
  YStd = sqrt(dy2 / (N - 1));
}