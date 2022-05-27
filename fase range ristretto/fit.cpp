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
  c->SetGrid();

  TGraphErrors* tweeter =
      new TGraphErrors("tweeter_3sigma.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* woofer =
      new TGraphErrors("woofer_3sigma.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* source = new TGraphErrors("sorgente.txt", "%lg%lg%lg%lg", "");

  TF1* f_w = new TF1("f_w", sfas_w, 4100, 4500, 1);
  TF1* f_t = new TF1("f_t", sfas_t, 4100, 4500, 1);
  TF1* f_s = new TF1("f_s", constant, 4100, 4500, 1);

  tweeter->Fit("f_t", "EX0, Q");
  TF1* fitTweeter = tweeter->GetFunction("f_t");

  woofer->Fit("f_w", "EX0, Q");
  TF1* fitWoofer = woofer->GetFunction("f_w");

  source->Fit("f_s", "EX0, Q");
  TF1* fitSource = source->GetFunction("f_s");

  double const chi_t = fitTweeter->GetChisquare() / fitTweeter->GetNDF();
  double const chi_w = fitWoofer->GetChisquare() / fitWoofer->GetNDF();
  double const chi_s = fitSource->GetChisquare() / fitSource->GetNDF();
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

  double x[1] = {f0};
  double y[1] = {0};
  double ex[1] = {d_f0};
  double ey[1] = {0};
  TGraphErrors* res1 = new TGraphErrors(1, x, y, ex, ey);

  TF1* result_1 = new TF1("result_1", sum_function, 4100, 4500, 2);
  result_1->SetParameters(tau_c, tau_l);

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

  fitSource->SetLineColor(6);
  fitSource->SetLineWidth(2);

  result_1->SetLineColor(2);
  result_1->SetLineWidth(2);

  res1->SetLineColor(1);
  res1->SetMarkerColor(1);
  res1->SetMarkerSize(0.5);
  res1->SetMarkerStyle(kFullCircle);

  TMultiGraph* mg = new TMultiGraph();
  mg->SetTitle("Sfasamento - Dati elaborati");
  mg->Add(tweeter);
  mg->Add(woofer);
  mg->Add(res1);
  mg->GetXaxis()->SetTitle("Frequenza [Hz]");
  mg->GetYaxis()->SetTitle("Fase [gradi]");
  mg->Draw("ACP");
  fitTweeter->Draw("C, SAME");
  fitWoofer->Draw("C, SAME");
  fitSource->Draw("C, SAME");
  result_1->Draw("C, SAME");

  TLegend* leg1 = new TLegend(.1, .7, .3, .9);
  leg1->AddEntry(tweeter, "Tweeter");
  leg1->AddEntry(fitTweeter, "Fit Tweeter");
  leg1->AddEntry(woofer, "Woofer");
  leg1->AddEntry(fitWoofer, "Fit Woofer");
  leg1->AddEntry(fitSource, "Sorgente");
  leg1->AddEntry(result_1, "Somma fit");
  leg1->AddEntry(res1, "Frequenza di crossover");
  leg1->Draw("SAME");

  std::cout << "\ntau_l: " << tau_l << " +/- " << d_tau_l
            << "\ntau_c: " << tau_c << " +/- " << d_tau_c
            << "\nOffset: " << offset << " +/- " << d_offset << '\n';

  TCanvas* c1 =
      new TCanvas("c1", "Sum of tweeter and woofer", 200, 10, 1200, 800);
  c1->SetGrid();

  TGraphErrors* sum =
      new TGraphErrors("somma_woofer+tweeter.txt", "%lg%lg%lg%lg", "");

  TF1* f_sum = new TF1("f_sum", sum_function, 4100, 4500, 2);

  sum->Fit("f_sum", "EX0, Q");
  TF1* fitSum = sum->GetFunction("f_sum");

  double const chi_sum = fitSum->GetChisquare() / fitSum->GetNDF();
  double const tau_l1 = fitSum->GetParameter(1);
  double const tau_c1 = fitSum->GetParameter(0);
  double const d_tau_l1 = fitSum->GetParError(1) * sqrt(chi_sum);
  double const d_tau_c1 = fitSum->GetParError(0) * sqrt(chi_sum);
  double const f01 = 1 / (2 * M_PI * sqrt(tau_c1 * tau_l1));
  double const DQl1 = -tau_c1 / (4 * M_PI * pow(tau_c1 * tau_l1, 1.5));
  double const DQc1 = -tau_l1 / (4 * M_PI * pow(tau_c1 * tau_l1, 1.5));
  double const d_f01 = sqrt(pow(DQc1 * d_tau_c1, 2) + pow(DQl1 * d_tau_l1, 2));

  std::cout << "\nSum:\nChiSquare/NDOF: " << chi_sum << '\n';

  double w[1] = {f01};
  double z[1] = {0};
  double ew[1] = {d_f01};
  double ez[1] = {0};
  TGraphErrors* res2 = new TGraphErrors(1, w, z, ew, ez);

  sum->SetTitle("Acquisizione somma degli sfasasamenti");
  sum->SetLineColor(38);
  //sum->SetFillColor(7);
  sum->SetMarkerColor(kBlue);
  sum->SetMarkerSize(0.5);
  sum->SetMarkerStyle(kOpenCircle);
  sum->GetXaxis()->SetTitle("Frequenza [Hz]");
  sum->GetYaxis()->SetTitle("Fase [gradi]");
  fitSum->SetLineColor(2);
  fitSum->SetLineWidth(2);

  res2->SetLineColor(1);
  res2->SetMarkerColor(1);
  res2->SetMarkerSize(0.5);
  res2->SetMarkerStyle(kFullCircle);

  TMultiGraph* mg1 = new TMultiGraph();
  mg1->SetTitle("Acquisizione somma degli sfasasamenti");
  mg1->Add(sum);
  //mg1->Add(res2);
  mg1->GetXaxis()->SetTitle("Frequenza [Hz]");
  mg1->GetYaxis()->SetTitle("Fase [gradi]");
  mg1->Draw("ALP");
  res2->Draw("SAME");
  fitSum->Draw("C, SAME");

  TLegend* leg2 = new TLegend(.1, .7, .3, .9);
  leg2->AddEntry(sum, "Somma - Dati");
  leg2->AddEntry(fitSum, "Fit Somma");
  leg2->AddEntry(res2, "Frequenza di crossover");
  leg2->Draw("SAME");

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
