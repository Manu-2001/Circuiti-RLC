#include "function.hpp"

double eval(double x) { return (x > 0.) ? x : -x; }

bool printData(misura const& X, misura const& Y, std::string const& u) {
  if ((Y.p + Y.d >= X.p - X.d && Y.p - Y.d <= X.p + X.d) ||
      (Y.p + Y.d <= X.p - X.d && Y.p - Y.d >= X.p + X.d)) {
    std::cout << "\033[32m" << X.p << " ± " << X.d << ' ' << u << "\033[0m";

    return true;
  }

  std::cout << "\033[35m" << X.p << " ± " << X.d << ' ' << u
            << "   dis.: " << eval(Y.p - X.p) / X.d << " error\033[0m";

  return false;
}

bool printData(misura const& X, double Y, std::string const& u) {
  if (Y >= X.p - X.d && Y <= X.p + X.d) {
    std::cout << "\033[32m" << X.p << " ± " << X.d << ' ' << u << "\033[0m";

    return true;
  }

  std::cout << "\033[35m" << X.p << " ± " << X.d << ' ' << u
            << "   dis.: " << eval(Y - X.p) / X.d << " error\033[0m";

  return false;
}

void printData(misura const& X, std::string const& u) {
  std::cout << X.p << " ± " << X.d << ' ' << u;
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

int Intersezione(misura& X1, misura& X2, misura& Y1, misura& Y2, TF1* const& f,
                 TF1* const& g, bool quad) {
  double const* fp = f->GetParameters();
  double const* gp = g->GetParameters();
  double const* dfp = f->GetParErrors();
  double const* dgp = g->GetParErrors();

  double const a = fp[0] - gp[0];
  double const b = fp[1] - gp[1];
  double const c = fp[2] - gp[2];

  double fp0{};
  double fp1{};
  double fp2{};
  double gp0{};
  double gp1{};
  double gp2{};

  double const delta = b * b - 4 * a * c;

  if (delta < 0. || a == 0.) {
    return EXIT_FAILURE;
  }

  // Intersezioni X
  X1.p = (-b + sqrt(delta)) / (2 * a);
  X2.p = -(b + sqrt(delta)) / (2 * a);

  // calcolo delle derivate per X1
  fp0 = eval(c / (a * sqrt(delta)) + (-b + sqrt(delta)) / (2 * pow(a, 2)));
  fp1 = eval((b / sqrt(delta) - 1) / (2 * a));
  fp2 = 1 / sqrt(delta);
  gp0 = fp0;
  gp1 = eval((1 - b / sqrt(delta)) / (2 * a));
  gp2 = fp2;

  // errore in quadratura o lineare per X1
  if (quad) {
    X1.d = sqrt(pow(fp0 * dfp[0], 2) + pow(fp1 * dfp[1], 2) +
                pow(fp2 * dfp[2], 2) + pow(gp0 * dgp[0], 2) +
                pow(gp1 * dgp[1], 2) + pow(gp2 * dgp[2], 2));
  } else {
    X1.d = fp0 * dfp[0] + fp1 * dfp[1] + fp2 * dfp[2] + gp0 * dgp[0] +
           gp1 * dgp[1] + gp2 * dgp[2];
  }

  // calcolo delle derivate per X2
  fp0 = eval(c / (a * sqrt(delta)) + (b + sqrt(delta)) / (2 * pow(a, 2)));
  fp1 = eval((b / sqrt(delta) + 1) / (2 * a));
  gp0 = fp0;
  gp1 = eval((1 + b / sqrt(delta)) / (2 * a));

  // errore in quadratura o lineare per X2
  if (quad) {
    X2.d = sqrt(pow(fp0 * dfp[0], 2) + pow(fp1 * dfp[1], 2) +
                pow(fp2 * dfp[2], 2) + pow(gp0 * dgp[0], 2) +
                pow(gp1 * dgp[1], 2) + pow(gp2 * dgp[2], 2));
  } else {
    X2.d = fp0 * dfp[0] + fp1 * dfp[1] + fp2 * dfp[2] + gp0 * dgp[0] +
           gp1 * dgp[1] + gp2 * dgp[2];
  }

  // Intersezioni Y
  Y1.p = f->Eval(X1.p);
  Y2.p = f->Eval(X2.p);

  Y1.d = 0.;
  Y2.d = 0.;
  
  return 0;
}
