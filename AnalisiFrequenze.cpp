#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"

// Dati: frequenza range ristretto 3.75 Hz - 4.75 Hz
// Sorgente: sinusoide di ampiezza 5V, sweep con salti di 10Hz
// Campionamento: 300 KHz
// CrossOver trovato: 4055.99 Hz ± /

void SmallRange() {
  TGraph* Tweeter = new TGraph("tweeter.txt", "%lg%lg", "");
  TGraph* Woofer = new TGraph("woofer.txt", "%lg%lg", "");

  TF1* quad = new TF1("quad", "[0]*x*x + [1]*x + [2]", 3000, 5000);

  Tweeter->Fit("quad", "R");
  TF1* FitTweeter = Tweeter->GetFunction("quad");

  TCanvas* TResult = new TCanvas("TResult", "Tweeter fit");
  Tweeter->Draw();
  FitTweeter->Draw("SAME");

  Woofer->Fit("quad", "R");
  TF1* FitWoofer = Woofer->GetFunction("quad");

  TCanvas* WResult = new TCanvas("WResult", "Woofer fit");
  Woofer->Draw();
  FitWoofer->Draw("SAME");

  TCanvas* Confronto = new TCanvas("Confronto", "Woofer fit");
  FitTweeter->Draw();
  FitWoofer->Draw("SAME");

  double const ta = FitTweeter->GetParameter(0);
  double const tb = FitTweeter->GetParameter(1);
  double const tc = FitTweeter->GetParameter(2);

  double const wa = FitWoofer->GetParameter(0);
  double const wb = FitWoofer->GetParameter(1);
  double const wc = FitWoofer->GetParameter(2);

  double const a = ta - wa;
  double const b = tb - wb;
  double const c = tc - wc;

  double const delta = b * b - 4 * a * c;

  if (delta > 0 && a != 0.) {
    double const x1 = -(b + sqrt(delta)) / (2 * a);
    double const x2 = (-b + sqrt(delta)) / (2 * a);
    std::cout << "\nZERI:\n\tx1 = " << x1 << " Hz\tx2 = " << x2 << " Hz.\n";
  }
}
