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

  double const chi_t = fitTweeter->GetChisquare() / fitTweeter->GetNDF();
  double const chi_w = fitWoofer->GetChisquare() / fitWoofer->GetNDF();
  double const chi_s = fitSource->GetChisquare() / fitSource->GetNDF();

  std::cout << "Tweeter:\nChiSquare/NDOF: " << chi_t << '\n'
            << "Woofer:\nChiSquare/NDOF: " << chi_w << '\n'
            << "Source:\nChiSquare/NDOF: " << chi_s << '\n';

  tweeter->SetLineColor(1);
  tweeter->SetMarkerColor(4);
  tweeter->SetMarkerSize(0.5);
  tweeter->SetMarkerStyle(kOpenCircle);
  fitTweeter->SetLineColor(38);
  fitTweeter->SetLineWidth(2);

  woofer->SetLineColor(1);
  woofer->SetMarkerColor(3);
  woofer->SetMarkerSize(0.5);
  woofer->SetMarkerStyle(kOpenSquare);
  fitWoofer->SetLineColor(8);
  fitWoofer->SetLineWidth(2);

  source->SetLineColor(1);
  source->SetMarkerColor(2);
  source->SetMarkerSize(0.5);
  source->SetMarkerStyle(kOpenTriangleUp);
  fitSource->SetLineColor(6);
  fitSource->SetLineWidth(2);

  TMultiGraph* mg = new TMultiGraph();
  mg->SetTitle("Tweeter and Woofer");
  mg->Add(tweeter);
  mg->Add(woofer);
  mg->Add(source);
  mg->Draw("ALPX");
  fitTweeter->Draw("SAME");
  fitWoofer->Draw("SAME");
  fitSource->Draw("SAME");

  TLegend* leg1 = new TLegend(.1, .7, .3, .9);
  leg1->AddEntry(tweeter, "Tweeter");
  leg1->AddEntry(fitTweeter, "Tweeter fit");
  leg1->AddEntry(woofer, "Woofer");
  leg1->AddEntry(fitWoofer, "Woofer fit");
  leg1->AddEntry(source, "Source");
  leg1->AddEntry(fitSource, "Source fit");
  leg1->Draw("SAME");

  double const tau_l = fitWoofer->GetParameter(0);
  double const tau_c = fitTweeter->GetParameter(0);
  double const d_tau_l = fitWoofer->GetParError(0) * sqrt(chi_t);
  double const d_tau_c = fitTweeter->GetParError(0) * sqrt(chi_w);
  double const offset = fitSource->GetParameter(0);
  double const d_offset = fitSource->GetParError(0) * sqrt(chi_s);
  double const f0 = 1 / (2 * M_PI * sqrt(tau_c * tau_l));

  double DQl = -tau_c / (4 * M_PI * pow(tau_c * tau_l, 1.5));
  double DQc = -tau_l / (4 * M_PI * pow(tau_c * tau_l, 1.5));

  double d_f0 = sqrt(pow(DQc * d_tau_c, 2) + pow(DQl * d_tau_l, 2));

  std::cout << "\ntau_l: " << tau_l << " +/- " << d_tau_l
            << "\ntau_c: " << tau_c << " +/- " << d_tau_c
            << "\nOffset: " << offset << " +/- " << d_offset << '\n';

  TCanvas* c1 =
      new TCanvas("c1", "Sum of tweeter and woofer", 200, 10, 1200, 800);

  TGraphErrors* sum =
      new TGraphErrors("somma_woofer+tweeter_3sigma.txt", "%lg%lg%lg%lg", "");

  TF1* f_sum = new TF1("f_sum", sum_function, 1800, 8000, 2);

  sum->Fit("f_sum", "EX0, Q");
  TF1* fitSum = sum->GetFunction("f_sum");

  double const chi_sum = fitSum->GetChisquare() / fitSum->GetNDF();

  std::cout << "\nSum:\nChiSquare/NDOF: " << chi_sum << '\n';

  TF1* check_function = new TF1("check_function", sum_function, 1800, 8000, 2);
  check_function->SetParameters(tau_c, tau_l);

  sum->SetTitle("Sum");
  sum->SetLineColor(1);
  sum->SetMarkerColor(kBlue);
  sum->SetMarkerSize(0.5);
  sum->SetMarkerStyle(kOpenCircle);
  sum->GetXaxis()->SetTitle("Frequenza [Hz]");
  sum->GetYaxis()->SetTitle("Fase [deg]");
  fitSum->SetLineColor(2);
  fitSum->SetLineWidth(2);
  check_function->SetLineColor(kOrange);
  check_function->SetLineWidth(2);

  sum->Draw("ALPX");
  fitSum->Draw("SAME");
  check_function->Draw("SAME");

  TLegend* leg2 = new TLegend(.1, .7, .3, .9);
  leg2->AddEntry(sum, "Sum acquisition");
  leg2->AddEntry(fitSum, "Sum fit");
  leg2->AddEntry(check_function, "Sum of fit functions on tweeter and woofer");
  leg2->Draw("SAME");

  double const tau_l1 = fitSum->GetParameter(1);
  double const tau_c1 = fitSum->GetParameter(0);
  double const d_tau_l1 = fitSum->GetParError(1) * sqrt(chi_sum);
  double const d_tau_c1 = fitSum->GetParError(0) * sqrt(chi_sum);
  double const f01 = 1 / (2 * M_PI * sqrt(tau_c1 * tau_l1));

  double const DQl1 = -tau_c1 / (4 * M_PI * pow(tau_c1 * tau_l1, 1.5));
  double const DQc1 = -tau_l1 / (4 * M_PI * pow(tau_c1 * tau_l1, 1.5));

  double const d_f01 = sqrt(pow(DQc1 * d_tau_c1, 2) + pow(DQl1 * d_tau_l1, 2));

  std::cout << "\ntau_l: " << tau_l1 << " +/- " << d_tau_l1
            << "\ntau_c: " << tau_c1 << " +/- " << d_tau_c1 << '\n';

  double const L = 0.0489;
  double const dL = 0.0005;
  double const C = 32.0e-9;
  double const dC = 0.3e-9;

  double const DQl2 = -C / (4 * M_PI * pow(L * C, 1.5));
  double const DQc2 = -L / (4 * M_PI * pow(L * C, 1.5));

  double const f02 = 1 / (2 * M_PI * sqrt(C * L));
  double const d_f02 = sqrt(pow(DQc2 * dC, 2) + pow(DQl2 * dL, 2));

  std::cout << "\nFrequenza di crossover del grafico 1: " << f0 << " +/- "
            << d_f0 << "\nFrequenza di crossover del grafico 2: " << f01
            << " +/- " << d_f01 << "\nFrequenza di crossover attesa: " << f02
            << " +/- " << d_f02 << '\n';

  // myFile->Write();
  // myFile->Close();
}
