#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "myFunction.hpp"

// Dati: frequenza range ristretto 3.75 kHz - 4.75 kHz
// Sorgente: sinusoide di ampiezza 5V, sweep con salti di 10Hz
// Campionamento: 300 KHz
// Analisi da fit parabolico:
//  - CrossOver trovato: (4052.0 ± 0.9) Hz

R__LOAD_LIBRARY(myFunction_cpp.so);

int SmallRange(bool draw = false) {
  double f{};
  double fStd{};
  double V{};
  double VStd{};

  Statistic(f, V, fStd, VStd);
  std::cout << "\nAnalisi rumore per la fase:"
            << "\n Frequenza: media: " << f << "\tDeviazione Standard: " << fStd
            << ".\n Diff. di potenziale: media: " << V
            << "\tDeviazione Standard: " << VStd << ".\n";

  TGraph* Tweeter = new TGraph("tweeter.txt", "%lg%lg", "");
  TGraph* Woofer = new TGraph("woofer.txt", "%lg%lg", "");

  TF1* parabola = new TF1("parabola", "[0]*x*x + [1]*x + [2]", 3000., 5000.);

  Tweeter->Fit("parabola", "R");
  TF1* FitTweeter = Tweeter->GetFunction("parabola");

  Woofer->Fit("parabola", "R");
  TF1* FitWoofer = Woofer->GetFunction("parabola");

  if (draw) {
    TCanvas* TResult = new TCanvas("TResult", "Tweeter fit");
    Tweeter->Draw();
    FitTweeter->Draw("SAME");

    TCanvas* WResult = new TCanvas("WResult", "Woofer fit");
    Woofer->Draw();
    FitWoofer->Draw("SAME");

    TCanvas* Confronto = new TCanvas("Confronto", "Intersezione dei fit");
    FitTweeter->Draw();
    FitWoofer->Draw("SAME");
  }

  double const* Tpar = FitTweeter->GetParameters();
  double const* Wpar = FitWoofer->GetParameters();
  double const* TparError = FitTweeter->GetParErrors();
  double const* WparError = FitWoofer->GetParErrors();

  double const a = Tpar[0] - Wpar[0];
  double const b = Tpar[1] - Wpar[1];
  double const c = Tpar[2] - Wpar[2];

  double const da = TparError[0] + WparError[0];
  double const db = TparError[1] + WparError[1];
  double const dc = TparError[2] + WparError[2];

  double const delta = b * b - 4 * a * c;

  if (delta < 0.) {
    return EXIT_FAILURE;
  }

  double const f1 = (a == 0.) ? -c / a : (-b + sqrt(delta)) / (2 * a);
  double const f2 = (a == 0.) ? -c / a : -(b + sqrt(delta)) / (2 * a);

  double const df1 =
      (a == 0.) ? eval(c / (pow(a, 2))) * da + eval(1 / a) * dc
                : eval(0.5 * b - 0.5 * sqrt(delta) - a * c / sqrt(delta)) * da /
                          pow(a, 2) +
                      eval(0.5 / a * (b / sqrt(delta) - 1)) * db +
                      eval(1 / sqrt(delta)) * dc;

  double const df2 =
      (a == 0.) ? eval(c / (pow(a, 2))) * da + eval(1 / a) * dc
                : eval(0.5 * b + 0.5 * sqrt(delta) + a * c / sqrt(delta)) * da /
                          pow(a, 2) +
                      0.5 * eval(1 / a + b / sqrt(delta) / a) * db +
                      eval(1 / sqrt(delta)) * dc;

  std::cout << "\nCrossOver:\n  f1: ";
  printData(f1, df1, 4023, 30, "Hz");
  std::cout << "\n  f2: ";
  printData(f2, df2, 4023, 30, "Hz");
  std::cout<<'\n';
  return 0;
}