#include <iostream>
#include <string>

#include "TCanvas.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"

void makeGraph(int dati = -1) {
  TGraph* Sorgente = new TGraph("sorgente.txt", "%lg%lg", "");
  TGraph* Tweeter = new TGraph("tweeter.txt", "%lg%lg", "");
  TGraph* Woofer = new TGraph("woofer.txt", "%lg%lg", "");

  TCanvas* Grafico = new TCanvas("Graph", "Graph");
  
  TLegend* Legenda = new TLegend(.1, .7, .3, .9);

  std::string Title{};

  if (!(dati == 0 || dati == 1 || dati == 2)) {
    std::cout << "\nQuali dati stai usando?"
              << "\n - 0: Sotto il CrossOver - 2.5kHz"
              << "\n - 1: CrossOver - 4.023kHz "
              << "\n - 2: Sopra il CrossOver - 6.25kHz\t";

    do {
      std::cout << " (0-1-2)?: ";
      std::cin >> dati;
    } while (!(dati == 0 || dati == 1 || dati == 2));
  }

  switch (dati) {
    case 0:
      Title = "Comportamento del filtro a 2.5kHz";
      break;
    case 1:
      Title = "Comportamento del filtro a 4.023kHz";
      break;
    case 2:
      Title = "Comportamento del filtro a 6.25kHz";
      break;
  }

  Grafico->SetGrid();

  Sorgente->SetTitle(Title.c_str());
  Sorgente->GetYaxis()->SetRangeUser(-5, 5);
  Sorgente->GetXaxis()->SetTitle("tempo (s)");
  Sorgente->GetYaxis()->SetTitle("Tensione (V)");
  Sorgente->GetXaxis()->CenterTitle(true);
  Sorgente->GetYaxis()->CenterTitle(true);
  Sorgente->SetLineColor(kViolet - 1);
  Sorgente->SetLineWidth(2);
  Sorgente->Draw("ALP");

  Tweeter->SetLineColor(kBlue);
  Tweeter->SetLineWidth(2);
  Tweeter->Draw("SAME");

  Woofer->SetLineColor(kGreen + 1);
  Woofer->SetLineWidth(2);
  Woofer->Draw("SAME");

  Legenda->AddEntry(Sorgente, "Sorgente");
  Legenda->AddEntry(Tweeter, "Tweeter");
  Legenda->AddEntry(Woofer, "Woofer");
  Legenda->Draw("SAME");
}