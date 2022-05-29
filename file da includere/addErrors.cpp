#include <array>
#include <fstream>
#include <iostream>
#include <string>

// il file da aprire deve avere per forza due colonne di dati e l'ultima riga
// deve essere  un 'a capo', no dati.
//...                          |  ...
//14949.962383	1.029530       |  14949.976304	1.029512     
//14949.976304	1.029512       |  15000.082348	1.026365
//15000.082348	1.026365    no |                          ok

int main() {
  double x{};
  double y{};
  double dx{};
  double dy{};

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
  if (!source.is_open()) {
    return EXIT_FAILURE;
  }

  std::cout << "dx: ";
  std::cin >> dx;
  std::cout << "dy: ";
  std::cin >> dy;

  while (true) {
    source >> x >> y;

    if (!source.good()) {
      break;

    } else {
      line = std::to_string(x);
      line += {"\t"};
      line += std::to_string(y);
      line += {"\t"};
      line += std::to_string(dx);
      line += {"\t"};
      line += std::to_string(dy);
      line += {"\n"};

      create << line;
    }

  }  // fine ciclo while

  source.close();
  create.close();
}
