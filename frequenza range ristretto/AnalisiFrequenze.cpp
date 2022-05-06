#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "myFunction.hpp"

// Dati: frequenza range ristretto 3.75 kHz - 4.75 kHz
// Sorgente: sinusoide di ampiezza 5V, sweep con salti di 10Hz
// Campionamento: 300 KHz
// Analisi da fit parabolico CrossOver:
//  - con errori in (f,V): (4052 ± 61) Hz
//  - senza errori in (f,V): (4052.0 ± 0.9) Hz
// Dall'analisi sul rumore:
//  - Frequenza (Hz):   Media: 4022.96	std: 0.0527788
//  - ddp (V):          media: 1.39167	std: 0.000447848
//  - (:) Freq attesa ~ 4023 Hz
//  - (:) Amp. attesa ~ 1.5 V + caduta causata dai 50 Ohm generatore

R__LOAD_LIBRARY(myFunction_cpp.so);

int SmallRange(bool draw = false) {
  TGraphErrors* Tweeter = new TGraphErrors("tweeter.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* Woofer = new TGraphErrors("woofer.txt", "%lg%lg%lg%lg", "");
  TF1* parabola = new TF1("parabola", "[0]*x*x + [1]*x + [2]", 3000., 5000.);

  char s{};

  Tweeter->Fit("parabola", "R");
  TF1* FitTweeter = Tweeter->GetFunction("parabola");

  Woofer->Fit("parabola", "R");
  TF1* FitWoofer = Woofer->GetFunction("parabola");

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

  std::cout << "\nAnalizzare il rumore? (S/N): ";
  std::cin >> s;

  std::cout << '\n';

  if (s == 's' || s == 'S') {
    double f{};
    double fStd{};
    double V{};
    double VStd{};

    Statistic(f, V, fStd, VStd, "rumore.txt");

    std::cout << "Analisi rumore:"
              << "\n Frequenza: media: " << f << "\tdev. std: " << fStd
              << "\n ddp:       media: " << V << "\tdev. std: " << VStd
              << "\nConsidera che le ";
  }

  std::cout
      << "Dev. std usate come errori nei dati sono:\n - df: 0.0527788\n - "
         "dV: 0.000447848\n";

  std::cout << "\nCrossOver:\n  f1: ";
  printData(f1, df1, 4023, "Hz");
  std::cout << "\n  f2: ";
  printData(f2, df2, 4023, "Hz");
  std::cout << '\n';

  if (draw) {
    TCanvas* TResult = new TCanvas("TResult", "Tweeter fit");
    Tweeter->SetTitle("Tweeter fit");
    Tweeter->GetXaxis()->SetTitle("Frequency [Hz]");
    Tweeter->GetYaxis()->SetTitle("ddp [V]");
    FitTweeter->SetLineColor(kOrange + 1);
    Tweeter->Draw();
    FitTweeter->Draw("SAME");
    TLegend* TweeterLegend = new TLegend(.1, .7, .3, .9, "Legend");
    TweeterLegend->AddEntry(Tweeter, "Tweeter data");
    TweeterLegend->AddEntry(FitTweeter, "Parabolic fit");
    TweeterLegend->Draw("SAME");

    TCanvas* WResult = new TCanvas("WResult", "Woofer fit");
    Woofer->SetTitle("Tweeter fit");
    Woofer->GetXaxis()->SetTitle("Frequency [Hz]");
    Woofer->GetYaxis()->SetTitle("ddp [V]");
    Woofer->Draw();
    FitWoofer->SetLineColor(kGreen + 1);
    FitWoofer->Draw("SAME");
    TLegend* WooferLegend = new TLegend(.1, .7, .3, .9, "Legend");
    WooferLegend->AddEntry(Woofer, "Woofer data");
    WooferLegend->AddEntry(FitWoofer, "Parabolic fit");
    WooferLegend->Draw("SAME");

    TCanvas* Confronto = new TCanvas("Confronto", "Intersezione dei fit");
    Tweeter->SetTitle("Confronto woofer e tweeter");
    Tweeter->GetYaxis()->SetRangeUser(2.50, 3.05);
    Tweeter->Draw();
    FitTweeter->Draw("SAME");
    Woofer->Draw("SAME");
    FitWoofer->Draw("SAME");
    TLegend* ConfrontoLegend = new TLegend(.1, .7, .3, .9, "Legend");
    ConfrontoLegend->AddEntry(Tweeter, "Tweeter data");
    ConfrontoLegend->AddEntry(FitTweeter, "Parabolic fit");
    ConfrontoLegend->AddEntry(Woofer, "Woofer data");
    ConfrontoLegend->AddEntry(FitWoofer, "Parabolic fit");
    ConfrontoLegend->Draw("SAME");
  }

  return 0;
}