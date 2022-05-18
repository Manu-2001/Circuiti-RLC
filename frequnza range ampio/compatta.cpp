#include <array>
#include <fstream>
#include <iostream>
#include <string>

// il file da aprire deve avere per forza due colonne di dati e l'ultima riga
// deve essere di dati, non un 'a capo'
//...                          |  ...
//14949.962383	1.029530       |  14949.976304	1.029512     
//14949.976304	1.029512       |  15000.082348	1.026365
//15000.082348	1.026365    ok |                          così no

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
  bool ifnf{false};

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
    xm = 0.;
    ym = 0.;

    // leggi i dati ripetuti
    for (x = firstX, y = firstY; !stop; ++x, ++y) {
      if (x == firstX && ifnf) {
        *x = xtmp;
        *y = ytmp;
        ++x;
        ++y;
        N = 1;
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
    }  // fine ciclo for

    // fai la media degli N valori
    for (x = firstX, y = firstY; x != firstX + N; ++x, ++y) {
      xm += *x;
      ym += *y;
    }

    // medie
    xm = xm / (double)N;
    ym = ym / (double)N;

    // creo la riga
    line = std::to_string(xm);
    line += {"\t"};
    line += std::to_string(ym);
    line += {"\n"};

    // se devi uscire assicurati di non dimenticarti l'ultimo
    // valore temporaneo
    if (esci && xtmp != 0.) {
      line += std::to_string(xtmp);
      line += {"\t"};
      line += std::to_string(ytmp);
      line += {"\n"};
    }
    
    // scrivo nel file
    create << line;

    // bruttino ma ci vuole, è un controllo
    ifnf = true;
  }  // fine ciclo while

  source.close();
  create.close();
}
