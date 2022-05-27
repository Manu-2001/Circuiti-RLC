#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
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

/* questa cosa serve solo a fare il grafico dei dati attesi vs quello che
   abbiamo acquisito*/

void dati_attesi() {
  double const L = 0.0489;
  double const C = 32.0e-9;
  double const RL = 997.9 + 150.1;
  double const RC = 999.9 + 150.1;
  double const tau_l2 = L / RL;
  double const tau_c2 = RC * C;

  TCanvas* attes = new TCanvas("attes", "Dati attesi", 200, 10, 1200, 800);
  attes->SetGrid();

  TGraphErrors* tweeter =
      new TGraphErrors("tweeter_3sigma.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* woofer =
      new TGraphErrors("woofer_3sigma.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* source =
      new TGraphErrors("sorgente_3sigma.txt", "%lg%lg%lg%lg", "");

  TF1* a_w = new TF1("a_w", sfas_w, 1800, 8000, 1);
  a_w->SetParameter(0, tau_l2);
  TF1* a_t = new TF1("a_t", sfas_t, 1800, 8000, 1);
  a_t->SetParameter(0, tau_c2);
  TF1* expec = new TF1("expec", sum_function, 1800, 8000, 2);
  expec->SetParameters(tau_c2, tau_l2);

  TF1* f_s = new TF1("f_s", constant, 1800, 8000, 1);
  source->Fit("f_s", "EX0, Q");
  TF1* fitSource = source->GetFunction("f_s");

  tweeter->SetLineColor(1);
  tweeter->SetMarkerColor(4);
  tweeter->SetMarkerSize(0.5);
  tweeter->SetMarkerStyle(kOpenCircle);
  a_t->SetLineColor(38);
  a_t->SetLineWidth(2);

  woofer->SetLineColor(1);
  woofer->SetMarkerColor(3);
  woofer->SetMarkerSize(0.5);
  woofer->SetMarkerStyle(kOpenSquare);
  a_w->SetLineColor(8);
  a_w->SetLineWidth(2);

  fitSource->SetLineColor(6);
  fitSource->SetLineWidth(2);

  expec->SetLineColor(2);
  expec->SetLineWidth(2);

  TMultiGraph* mg1 = new TMultiGraph();
  mg1->SetTitle("Sfasamento - Dati sperimentali");
  mg1->Add(tweeter);
  mg1->Add(woofer);
  mg1->GetXaxis()->SetTitle("Frequenza [Hz]");
  mg1->GetYaxis()->SetTitle("Fase [gradi]");
  mg1->Draw("APX");
  a_t->Draw("SAME");
  a_w->Draw("SAME");
  fitSource->Draw("SAME");
  expec->Draw("SAME");

  TLegend* leg1 = new TLegend(.1, .7, .3, .9);
  leg1->AddEntry(tweeter, "Tweeter");
  leg1->AddEntry(a_t, "Tweeter atteso");
  leg1->AddEntry(woofer, "Woofer");
  leg1->AddEntry(a_w, "Woofer atteso");
  leg1->AddEntry(fitSource, "Sorgente");
  leg1->AddEntry(expec, "Somma attesa");
  leg1->Draw("SAME");

  double const f01 = 1 / (2 * M_PI * sqrt(tau_l2 * tau_c2));
  double const f02 = 1 / (2 * M_PI * sqrt(C * L));

  std::cout << "\nFrequenza di crossover del grafico: " << f01
            << "\nFrequenza di crossover attesa: " << f02 << '\n';
}
