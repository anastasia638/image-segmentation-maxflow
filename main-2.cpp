#include "pixel.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    // vériification des arguments
    if (argc < 2) {
        return 1;
    }
    // chargemenT de l’image et construction du graphe
    Graphe g(argv[1]);

    // boucle principale de Ford-Fulkerson
    int taille = g.L * g.C + 2;
    std::vector<Pixel*> predecesseur(taille, nullptr);

    int flotMax = 0;
    int iteration = 1;

    std::cout << " Début de la boucle Ford-Fulkerson " << std::endl;
    while (g.chercherCheminAugmentant(predecesseur)) {
        int deltaMin = g.getDeltaMin(predecesseur);
        if (deltaMin == 0) {
            break;
        }

        std::cout << "Itération " << iteration
                  << " | delta = " << deltaMin << std::endl;

        g.augmenterFlot(predecesseur, deltaMin);
        flotMax += deltaMin;

        std::fill(predecesseur.begin(), predecesseur.end(), nullptr);
        iteration++;
    }

    std::cout << "--- Terminé Flot maximal trouvé : " << flotMax << std::endl;

    g.BinariserImage();
    g.sauvegarderImage("image_binaire.pgm");
    return 0;
}
