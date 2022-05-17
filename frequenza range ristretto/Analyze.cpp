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

R__LOAD_LIBRARY(function_cpp.so);

int Analyze() {
  TFile* file = new TFile("fitdata.root", "READ");

  TGraphErrors* Tweeter = (TGraphErrors*)file->Get("Tweeter");
  TGraphErrors* Woofer = (TGraphErrors*)file->Get("Woofer");
  TF1* FitTweeter = (TF1*)file->Get("FitTweeter");
  TF1* FitWoofer = (TF1*)file->Get("FitWoofer");

  misura X1{}, X2{}, Y1{}, Y2{};
  misura fc = {4020, 40};
  misura fcq = {4023, 28};
  misura Yc = {2.8, 0.};

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
  std::cout << '\n';

  TCanvas* Confronto = new TCanvas("Confronto", "Intersezione dei fit");
  Confronto->SetGrid();

  Tweeter->SetTitle("Risposta in frequenza - Range ristretto");
  Tweeter->GetXaxis()->SetTitle("Frequenza (Hz)");
  Tweeter->GetXaxis()->CenterTitle(true);
  Tweeter->GetYaxis()->SetTitle("Ampiezza (V)");
  Tweeter->GetYaxis()->CenterTitle(true);
  Tweeter->GetYaxis()->SetRangeUser(2.65, 2.92);
  Tweeter->GetXaxis()->SetRangeUser(3740, 4280);
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
      new TGraphErrors(1, &X2.p, &Y2.p, &X2.d, &Y2.d);
  PointCrossOver->SetLineColor(kBlue);
  PointCrossOver->Draw("SAME");

  TGraphErrors* CrossOverAtteso =
      new TGraphErrors(1, &fc.p, &Yc.p, &fc.d, &Yc.d);
  CrossOverAtteso->SetLineColor(kRed);
  CrossOverAtteso->Draw("SAME");

  TLegend* ConfrontoLegend = new TLegend(.1, .7, .3, .9);
  ConfrontoLegend->AddEntry(Tweeter, "Tweeter");
  ConfrontoLegend->AddEntry(FitTweeter, "Tweeter fit");
  ConfrontoLegend->AddEntry(Woofer, "Woofer");
  ConfrontoLegend->AddEntry(FitWoofer, "Woofer fit");
  ConfrontoLegend->AddEntry(PointCrossOver,
                            "CrossOver fit (4067.4 +/- 0.6) Hz");
  ConfrontoLegend->AddEntry(CrossOverAtteso,
                            "CrossOver atteso (4020 +/- 40) Hz");
  ConfrontoLegend->Draw("SAME");

  return 0;
}