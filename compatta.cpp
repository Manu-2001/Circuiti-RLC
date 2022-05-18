#include <array>
#include <fstream>
#include <iostream>
#include <string>

int main() {
  using iterator = std::array<double, 5>::iterator;

  std::array<double, 10> X{};
  std::array<double, 10> Y{};

  iterator const firstX = X.begin();
  iterator const lastX = X.end();
  iterator const firstY = Y.begin();
  iterator x{};
  iterator y{};

  double xtmp{};
  double ytmp{};
  double xm{};
  double ym{};

  int N{};

  bool esci{false};
  bool stop{};
  bool isntfirst{false};

  std::string line{};
  std::string rowFile{};
  std::string newFile{};

  std::cout << "file sorgente: ";
  std::getline(std::cin, rowFile);

  std::cout << "nuovo file: ";
  std::getline(std::cin, newFile);

  std::ifstream source(rowFile);
  std::ofstream create(newFile);

  // fine inizializzazione oggetti, inizio istruzioni

  if (!(source.is_open() && create.is_open())) {
    return EXIT_FAILURE;
  }

  while (!esci) {
    stop = false;
    N = 0;
    xm = 0.;
    ym = 0.;

    // leggi i dati ripetuti
    for (x = firstX, y = firstY; !stop; ++x, ++y) {
      if (x == firstX && isntfirst) {
        *x = xtmp;
        *y = ytmp;
        ++x;
        ++y;
        ++N;
        xtmp = 0.;
        ytmp = 0.;
      }

      source >> *x >> *y;
      ++N;

      if (!source.good()) {
        stop = true;
        esci = true;
      }
      if (*x > *firstX + 5.) {
        xtmp = *x;
        ytmp = *y;
        --N;
        stop = true;
      }
    }

    // fai la media degli N valori
    for (x = firstX, y = firstY; x != firstX + N; ++x, ++y) {
      xm += *x;
      ym += *y;
    }

    // medie
    xm = xm / N;
    ym = ym / N;

    // creo la riga
    line = std::to_string(xm);
    line += {"\t"};
    line += std::to_string(ym);
    line += {"\n"};

    // scrivo nel file la riga
    create << line;

    // se devi uscire assicurati di non dimenticarti l'ultimo
    // valore temporaneo
    if (esci && xtmp != 0.) {
      line = std::to_string(xtmp);
      line += {"\t"};
      line += std::to_string(ytmp);
      line += {"\n"};
      create << line;
    }

    // bruttino ma ci vuole, è un controllo
    isntfirst = true;
  }

  source.close();
  create.close();
}
