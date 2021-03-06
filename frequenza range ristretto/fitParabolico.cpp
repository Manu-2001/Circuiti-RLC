#include "TF1.h"
#include "TGraphErrors.h"
#include "TFile.h"

void fitParabolico(){
  TFile* myFile = new TFile("fitParabData.root", "RECREATE");

  TGraphErrors* Tweeter = new TGraphErrors("tweeter.txt", "%lg%lg%lg%lg", "");
  TGraphErrors* Woofer = new TGraphErrors("woofer.txt", "%lg%lg%lg%lg", "");

  TF1* parabola = new TF1("parabola", "[0]*x*x + [1]*x + [2]", 3000., 5000.);

  Tweeter->Fit("parabola", "EX0");
  TF1* FitTweeter = Tweeter->GetFunction("parabola");

  Woofer->Fit("parabola", "EX0");
  TF1* FitWoofer = Woofer->GetFunction("parabola");

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