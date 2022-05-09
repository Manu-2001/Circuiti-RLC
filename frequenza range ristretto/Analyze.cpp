#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
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
// CrossOver atteso:
//  - errori in quadratura: (4023 ± 28) Hz
//  - errori lineari :      (4020 ± 40) Hz
// Analisi da fit parabolico CrossOver: (in accordo entrambe)
//  - con errori in (f, V): pro. in quad.(4052 ± 27) Hz
//                          prop. linear. (4052 ± 61) Hz
// Dall'analisi sul rumore:
//  - Frequenza (Hz):   Media: 4022.96	std: 0.0527788
//  - ddp (V):          media: 1.39167	std: 0.000447848
//  - (:) Freq attesa ~ 4023 Hz
//  - (:) Amp. attesa ~ 1.5 V + caduta causata dai 50 Ohm generatore

R__LOAD_LIBRARY(function_cpp.so);

int Analyze(bool draw = false) {
  TFile* file = new TFile("fitdata.root", "READ");

  TGraphErrors* Tweeter = (TGraphErrors*)file->Get("Tweeter");
  TGraphErrors* Woofer = (TGraphErrors*)file->Get("Woofer");
  TF1* FitTweeter = (TF1*)file->Get("FitTweeter");
  TF1* FitWoofer = (TF1*)file->Get("FitWoofer");

  misura X1{}, X2{}, Y1{}, Y2{};
  misura fc = {4020, 40};
  misura fcq = {4023, 28};
  misura Yc = {2.8, 0.};

  char s{};

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

  (void)IntersParab(X1, X2, Y1, Y2, FitTweeter, FitWoofer, true);

  std::cout << "\nCrossOver - errori quad:\n  f1: ";
  (void)printData(X1, fcq, "Hz");
  std::cout << "\n  f2: ";
  (void)printData(X2, fcq, "Hz");

  (void)IntersParab(X1, X2, Y1, Y2, FitTweeter, FitWoofer, false);

  std::cout << "\nCrossOver - errori linear:\n  f1: ";
  (void)printData(X1, fc, "Hz");
  std::cout << "\n  f2: ";
  (void)printData(X2, fc, "Hz");

  if (draw) {
    TCanvas* Confronto = new TCanvas("Confronto", "Intersezione dei fit");
    Confronto->SetGrid();

    Tweeter->SetTitle("Risposta in frequenza - Range ristretto");
    Tweeter->GetXaxis()->SetTitle("Frequenza (Hz)");
    Tweeter->GetXaxis()->CenterTitle(true);
    Tweeter->GetYaxis()->SetTitle("Ampiezza (V)");
    Tweeter->GetYaxis()->CenterTitle(true);
    Tweeter->GetYaxis()->SetRangeUser(2.50, 3.05);
    Tweeter->GetXaxis()->SetRangeUser(3700, 4800);
    Tweeter->Draw("ALP");

    FitTweeter->SetLineColor(kOrange + 1);
    FitTweeter->Draw("SAME");

    Woofer->SetTitle("Woofer fit");
    Woofer->GetXaxis()->SetTitle("Frequenza (Hz)");
    Woofer->GetXaxis()->CenterTitle(true);
    Woofer->GetYaxis()->SetTitle("Ampiezza (V)");
    Woofer->GetYaxis()->CenterTitle(true);
    Woofer->Draw("SAME");

    FitWoofer->SetLineColor(kGreen + 1);
    FitWoofer->Draw("SAME");

    TGraphErrors* PointCrossOver =
        new TGraphErrors(1, &X1.p, &Y1.p, &X1.d, &Y1.d);
    PointCrossOver->SetMarkerStyle(20);
    PointCrossOver->SetLineColor(kBlue);
    PointCrossOver->Draw("SAME");

    TGraphErrors* CrossOverAtteso =
        new TGraphErrors(1, &fc.p, &Yc.p, &fc.d, &Yc.d);
    CrossOverAtteso->SetMarkerStyle(20);
    CrossOverAtteso->SetLineColor(kRed);
    CrossOverAtteso->Draw("SAME");

    TLegend* ConfrontoLegend = new TLegend(.1, .7, .3, .9);
    ConfrontoLegend->AddEntry(Tweeter, "Tweeter");
    ConfrontoLegend->AddEntry(FitTweeter, "Tweeter fit");
    ConfrontoLegend->AddEntry(Woofer, "Woofer");
    ConfrontoLegend->AddEntry(FitWoofer, "Woofer fit");
    ConfrontoLegend->AddEntry(PointCrossOver, "CrossOver");
    ConfrontoLegend->AddEntry(CrossOverAtteso, "CrossOver atteso");
    ConfrontoLegend->Draw("SAME");
  }

  std::cout << '\n';
  return 0;
}