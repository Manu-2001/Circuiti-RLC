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

int Analyze() {
  TFile* file = new TFile("DataFit.root", "READ");

  TGraphErrors* Tweeter = (TGraphErrors*)file->Get("Tweeter");
  TGraphErrors* Woofer = (TGraphErrors*)file->Get("Woofer");

  TF1* FitTweeter = (TF1*)file->Get("FitTweeter");
  TF1* FitWoofer = (TF1*)file->Get("FitWoofer");

  TCanvas* Confronto = new TCanvas("Confronto", "Intersezione dei fit");
  Confronto->SetGrid();

  Tweeter->SetTitle("Risposta in frequenza - Range ampio");
  Tweeter->GetXaxis()->SetTitle("Frequenza (Hz)");
  Tweeter->GetXaxis()->CenterTitle(true);
  Tweeter->GetYaxis()->SetTitle("Ampiezza (V)");
  Tweeter->GetYaxis()->CenterTitle(true);
  Tweeter->GetYaxis()->SetRangeUser(0., 5.);
  Tweeter->GetXaxis()->SetRangeUser(1000., 15000.);
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

  TLegend* ConfrontoLegend = new TLegend(.1, .7, .3, .9);
  ConfrontoLegend->AddEntry(Tweeter, "Tweeter");
  ConfrontoLegend->AddEntry(FitTweeter, "Tweeter fit");
  ConfrontoLegend->AddEntry(Woofer, "Woofer");
  ConfrontoLegend->AddEntry(FitWoofer, "Woofer fit");
  ConfrontoLegend->Draw("SAME");

  return 0;
}