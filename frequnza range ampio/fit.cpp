#include <cmath>

#include "TF1.h"
#include "TGraphErrors.h"
#include "TFile.h"

Double_t fw(Double_t* x, Double_t* par) {
  return 4.77793 * 0.983 / sqrt(1 + pow(2 * M_PI * par[0] * x[0], 2));
}

Double_t ft(Double_t* x, Double_t* par) {
  return 4.77793 * 0.985 / sqrt(1 + 1 / pow(2 * M_PI * par[0] * x[0], 2));
}

void fit (){
  TFile* myFile = new TFile("DataFit.root", "RECREATE");

  TGraphErrors* Tweeter = new TGraphErrors("tweeter.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* Woofer = new TGraphErrors("woofer.txt", "%lg%lg%lg%lg", "");

  TF1* fwl = new TF1("fwl", fw, 1000., 15000., 1);
  TF1* ftc = new TF1("ftc", ft, 1000., 15000., 1);

  Woofer->Fit("fwl", "EX0");
  TF1* FitWoofer = Woofer->GetFunction("fwl");

  Tweeter->Fit("ftc", "EX0");
  TF1* FitTweeter = Tweeter->GetFunction("ftc");

  Tweeter->SetName("Tweeter");
  Woofer->SetName("Woofer");
  FitTweeter->SetName("FitTweeter");
  FitWoofer->SetName("FitWoofer");

  Tweeter->Write();
  Woofer->Write();
  FitTweeter->Write();
  FitWoofer->Write();
  
  myFile->Close();
}