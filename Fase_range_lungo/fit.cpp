#include <cmath>

#include "TFile.h"
#include "TF1.h"
#include "TGraphErrors.h"

Double_t sfas_w(Double_t* x, Double_t* par) {
  Float_t xx = x[0];
  Double_t val = -1 * atan(2 * M_PI * par[0] * xx);
  return val * 180 / M_PI;
}

Double_t sfas_t(Double_t* x, Double_t* par) {
  Float_t xx = x[0];
  Double_t val = atan(1/(2 * M_PI * par[0] * xx));
  return val * 180 / M_PI;
}

void fit() {
  TFile* myFile = new TFile("fitdata.root", "RECREATE");

  TGraphErrors* tweeter = new TGraphErrors("tweeter.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* woofer = new TGraphErrors("woofer.txt", "%lg%lg%lg%lg", "");

  TF1 *f_w = new TF1("f_w", sfas_w, 1800, 8000, 1);
  f_w->SetParameter(0, 5e-5);
  TF1 *f_t = new TF1("f_t", sfas_t, 1800, 8000, 1);
  f_t->SetParameter(0, 5e-5);

  tweeter->Fit("f_w", "EX0");
  TF1* fitTweeter = tweeter->GetFunction("f_w");

  woofer->Fit("f_t", "EX0");
  TF1* fitWoofer = woofer->GetFunction("f_t");

  myFile->Write();
  myFile->Close();
}