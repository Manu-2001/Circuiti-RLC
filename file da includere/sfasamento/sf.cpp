#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"

Double_t sss(Double_t* x, Double_t* par) {
  Float_t xx = x[0];
  Double_t val = par[1] * sin(xx + par[0] * M_PI / 180);
  return val;
}

void sf() {
  TCanvas* c = new TCanvas("c", "Tweeter and Woofer", 200, 10, 1200, 800);
  c->SetGrid();

  TGraphErrors* s1 = new TGraphErrors("sorgente1.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* s2 = new TGraphErrors("sorgente2.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* s3 = new TGraphErrors("sorgente3.txt", "%lg%lg%lg%lg", "");

  TF1* f = new TF1("f", sss, 0., 0.001, 2);
  f->SetParameters(1e-6, 2.4);
  f->SetParLimits(0, 1e-10, 1e-3);
  f->SetParLimits(1, 1.5, 3);

  s1->Fit("f", "EX0, Q, B");
  TF1* fun1 = s1->GetFunction("f");

  s2->Fit("f", "EX0, Q, B");
  TF1* fun2 = s2->GetFunction("f");

  s3->Fit("f", "EX0, Q, B");
  TF1* fun3 = s3->GetFunction("f");

  fun1->SetLineColor(38);
  fun1->SetLineWidth(2);

  fun2->SetLineColor(8);
  fun2->SetLineWidth(2);

  fun3->SetLineColor(6);
  fun3->SetLineWidth(2);

  TMultiGraph* mg = new TMultiGraph();
  mg->SetTitle("Sfasamento - Dati elaborati");
  mg->Add(s1);
  mg->Add(s2);
  mg->Add(s3);
  mg->Draw("ACPX");
  fun1->Draw("SAME");
  fun2->Draw("SAME");
  fun3->Draw("SAME");

  double const chi_1 = fun1->GetChisquare() / fun1->GetNDF();
  double const chi_2 = fun2->GetChisquare() / fun2->GetNDF();
  double const chi_3 = fun3->GetChisquare() / fun3->GetNDF();
  double const phi1 = fun1->GetParameter(0);
  double const phi2 = fun2->GetParameter(0);
  double const phi3 = fun3->GetParameter(0);
  double const d_phi1 = fun1->GetParError(0) /** sqrt(chi_1)*/;
  double const d_phi2 = fun2->GetParError(0) /** sqrt(chi_2)*/;
  double const d_phi3 = fun3->GetParError(0) /** sqrt(chi_3)*/;
  double const delta1 = phi2 - phi1;
  double const delta2 = phi3 - phi2;
  double const d_delta1 = sqrt(pow(phi1, 2) + pow(phi2, 2));
  double const d_delta2 = sqrt(pow(phi3, 2) + pow(phi2, 2));

  std::cout << "\nphi_1: " << phi1 << " +/- " << d_phi1 << "\nphi_2: " << phi2
            << " +/- " << d_phi2 << "\nphi_3: " << phi3 << " +/- " << d_phi3
            << "\ndelta1: " << delta1 << " +/- " << d_delta1
            << "\ndelta2: " << delta2 << " +/- " << d_delta2 << '\n';
}