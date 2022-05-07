#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "function.hpp"
#include "misura.hpp"

// Dati: frequenza range ristretto 3.75 kHz - 4.75 kHz
// Sorgente: sinusoide di ampiezza 5V, sweep con salti di 10Hz
// Campionamento: 300 KHz
// Analisi da fit parabolico CrossOver:
//  - con errori in (f, V): (4052 ± 61) Hz
// Dall'analisi sul rumore:
//  - Frequenza (Hz):   Media: 4022.96	std: 0.0527788
//  - ddp (V):          media: 1.39167	std: 0.000447848
//  - (:) Freq attesa ~ 4023 Hz
//  - (:) Amp. attesa ~ 1.5 V + caduta causata dai 50 Ohm generatore

R__LOAD_LIBRARY(function_cpp.so);

int SmallRange(bool draw = false) {
  TGraphErrors* Tweeter = new TGraphErrors("tweeter.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* Woofer = new TGraphErrors("woofer.txt", "%lg%lg%lg%lg", "");

  TF1* parabola = new TF1("parabola", "[0]*x*x + [1]*x + [2]", 3000., 5000.);

  misura X1{}, X2{}, Y1{}, Y2{};

  char s{};

  Tweeter->Fit("parabola", "R");
  TF1* FitTweeter = Tweeter->GetFunction("parabola");

  Woofer->Fit("parabola", "R");
  TF1* FitWoofer = Woofer->GetFunction("parabola");

  Intersezione(X1, X2, Y1, Y2, FitTweeter, FitWoofer, false);

  std::cout << "\nAnalizzare il rumore? (S/N): ";
  std::cin >> s;
  std::cout << '\n';

  if (s == 's' || s == 'S') {
    double fm{};
    double fStd{};
    double Vm{};
    double VStd{};

    Statistic(fm, Vm, fStd, VStd, "rumore.txt");

    std::cout << "Analisi rumore:"
              << "\n Frequenza: media: " << fm << "\tdev. std: " << fStd
              << "\n ddp:       media: " << Vm << "\tdev. std: " << VStd
              << "\nConsidera che le ";
  }

  std::cout
      << "Dev. std usate come errori nei dati sono:\n - df: 0.0527788\n - "
         "dV: 0.000447848\n";

  std::cout << "\nCrossOver:\n  f1: ";
  (void)printData(X1, 4023, "Hz");
  std::cout << "\n  V1: ";
  (void)printData(Y1, 2.784,"V");
  std::cout << "\n  f2: ";
  (void)printData(X2, 4023, "Hz");
  std::cout << "\n  V2: ";
  (void)printData(Y2, 2.784,"V");

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
    Woofer->SetTitle("Woofer fit");
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
    Tweeter->GetXaxis()->SetRangeUser(3700, 4800);
    Tweeter->Draw();
    FitTweeter->Draw("SAME");
    Woofer->Draw("SAME");
    FitWoofer->Draw("SAME");
    TGraphErrors* PointCrossOver =
        new TGraphErrors(1, &X1.p, &Y1.p, &X1.d, &Y1.d);
    PointCrossOver->SetMarkerStyle(20);
    PointCrossOver->SetLineColor(kBlue);
    PointCrossOver->Draw("SAME");
    TLegend* ConfrontoLegend = new TLegend(.1, .7, .3, .9, "Legend");
    ConfrontoLegend->AddEntry(Tweeter, "Tweeter data");
    ConfrontoLegend->AddEntry(FitTweeter, "Tweeter fit");
    ConfrontoLegend->AddEntry(Woofer, "Woofer data");
    ConfrontoLegend->AddEntry(FitWoofer, "Woofer fit");
    ConfrontoLegend->AddEntry(PointCrossOver, "CrossOver");
    ConfrontoLegend->Draw("SAME");
  }

  std::cout << '\n';
  return 0;
}