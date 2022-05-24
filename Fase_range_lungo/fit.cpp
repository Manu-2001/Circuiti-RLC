#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
// #include "TFile.h"
#include "TGraphErrors.h"

Double_t sfas_w(Double_t* x, Double_t* par) {
  Float_t xx = x[0];
  Double_t val = -1 * atan(2 * M_PI * par[0] * xx);
  return val * 180 / M_PI;
}

Double_t sfas_t(Double_t* x, Double_t* par) {
  Float_t xx = x[0];
  Double_t val = atan(1 / (2 * M_PI * par[0] * xx));
  return val * 180 / M_PI;
}

Double_t constant(Double_t* x, Double_t* par) {
  Float_t xx = x[0];
  Double_t val = par[0];
  return val * 180 / M_PI;
}

Double_t sum_function(Double_t* x, Double_t* par) {
  Float_t xx = x[0];
  Double_t val =
      atan(1 / (2 * M_PI * par[0] * xx)) - atan(2 * M_PI * par[1] * xx);
  return val * 180 / M_PI;
}

void fit() {
  // TFile* myFile = new TFile("fitdata.root", "RECREATE");

  TCanvas* c = new TCanvas("c", "Tweeter and Woofer", 200, 10, 1200, 800);

  TGraphErrors* tweeter =
      new TGraphErrors("tweeter_3sigma.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* woofer =
      new TGraphErrors("woofer_3sigma.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* source =
      new TGraphErrors("sorgente_3sigma.txt", "%lg%lg%lg%lg", "");

  TF1* f_w = new TF1("f_w", sfas_w, 1800, 8000, 1);
  TF1* f_t = new TF1("f_t", sfas_t, 1800, 8000, 1);
  TF1* f_s = new TF1("f_s", constant, 1800, 8000, 1);

  tweeter->Fit("f_t", "EX0, Q");
  TF1* fitTweeter = tweeter->GetFunction("f_t");

  woofer->Fit("f_w", "EX0, Q");
  TF1* fitWoofer = woofer->GetFunction("f_w");

  source->Fit("f_s", "EX0, Q");
  TF1* fitSource = source->GetFunction("f_s");

  std::cout << "Tweeter:\n"
            << "ChiSquare/NDOF: "
            << fitTweeter->GetChisquare() / fitTweeter->GetNDF() << '\n'
            << "Woofer:\n"
            << "ChiSquare/NDOF: "
            << fitWoofer->GetChisquare() / fitWoofer->GetNDF() << '\n'
            << "Source:\n"
            << "ChiSquare/NDOF: "
            << fitSource->GetChisquare() / fitSource->GetNDF() << '\n';

  tweeter->SetTitle("Tweeter");
  tweeter->SetLineColor(kGreen);
  tweeter->SetMarkerColor(kBlue);
  tweeter->SetMarkerSize(0.25);
  tweeter->SetMarkerStyle(kOpenCircle);
  tweeter->SetFillColor(38);
  // tweeter->GetXaxis()->SetTitle("Frequenza [Hz]");
  // tweeter->GetYaxis()->SetTitle("Fase [deg]");
  fitTweeter->SetLineColor(kRed);
  fitTweeter->SetLineWidth(2);

  woofer->SetTitle("Woofer");
  woofer->SetLineColor(kGreen);
  woofer->SetMarkerColor(kBlue);
  woofer->SetMarkerSize(0.25);
  woofer->SetMarkerStyle(kOpenCircle);
  woofer->SetFillColor(38);
  // woofer->GetXaxis()->SetTitle("Frequenza [Hz]");
  // woofer->GetYaxis()->SetTitle("Fase [deg]");
  fitWoofer->SetLineColor(kRed);
  fitWoofer->SetLineWidth(2);

  source->SetTitle("Source");
  source->SetLineColor(kGreen);
  source->SetMarkerColor(kBlue);
  source->SetMarkerSize(0.25);
  source->SetMarkerStyle(kOpenCircle);
  source->SetFillColor(38);
  // woofer->GetXaxis()->SetTitle("Frequenza [Hz]");
  // woofer->GetYaxis()->SetTitle("Fase [deg]");
  fitSource->SetLineColor(kRed);
  fitSource->SetLineWidth(2);

  TMultiGraph* mg = new TMultiGraph();
  mg->Add(tweeter);
  mg->Add(woofer);
  mg->Add(source);
  mg->Draw("ALP");
  fitTweeter->Draw("SAME");
  fitWoofer->Draw("SAME");
  fitSource->Draw("SAME");

  double tau_l = fitWoofer->GetParameter(0);
  double tau_c = fitTweeter->GetParameter(0);
  double d_tau_l = fitWoofer->GetParError(0);
  double d_tau_c = fitTweeter->GetParError(0);
  double offset = fitSource->GetParameter(0);
  double d_offset = fitSource->GetParError(0);
  double f0 = 1 / (2 * M_PI * sqrt(tau_c * tau_l));

  std::cout << "tau_l: " << tau_l << " +/- " << d_tau_l << "\ntau_c: " << tau_c
            << " +/- " << d_tau_c << "\nOffset: " << offset << " +/- "
            << d_offset << '\n';

  TCanvas* c1 =
      new TCanvas("c1", "Sum of tweeter and woofer", 200, 10, 1200, 800);

  TGraphErrors* sum =
      new TGraphErrors("somma_woofer+tweeter_3sigma.txt", "%lg%lg%lg%lg", "");

  TF1* f_sum = new TF1("f_sum", sum_function, 1800, 8000, 2);

  sum->Fit("f_sum", "EX0, Q");
  TF1* fitSum = sum->GetFunction("f_sum");

  std::cout << "Sum:\n"
            << "ChiSquare/NDOF: " << fitSum->GetChisquare() / fitSum->GetNDF()
            << '\n';

  TF1* check_function = new TF1("check_function", sum_function, 1800, 8000, 2);
  check_function->SetParameters(tau_c, tau_l);

  sum->SetTitle("Sum");
  sum->SetLineColor(kGreen);
  sum->SetMarkerColor(kBlue);
  sum->SetMarkerSize(0.25);
  sum->SetMarkerStyle(kOpenCircle);
  sum->SetFillColor(38);
  // sum->GetXaxis()->SetTitle("Frequenza [Hz]");
  // sum->GetYaxis()->SetTitle("Fase [deg]");
  fitSum->SetLineColor(kRed);
  fitSum->SetLineWidth(2);
  check_function->SetLineColor(kOrange);
  check_function->SetLineWidth(2);

  sum->Draw("ALP");
  f_sum->Draw("SAME");
  check_function->Draw("SAME");

  std::cout << "tau_l: " << f_sum->GetParameter(1) << " +/- "
            << f_sum->GetParError(1) << "\ntau_c: " << f_sum->GetParameter(0)
            << " +/- " << f_sum->GetParError(0) << '\n';

  double f01 =
      1 / (2 * M_PI * sqrt(f_sum->GetParameter(0) * f_sum->GetParameter(1)));

  std::cout << "\nFrequenza di crossover del grafico 1: " << f0
            << "\nFrequenza di crossover del grafico 2: " << f01 << '\n';

  // myFile->Write();
  // myFile->Close();
}