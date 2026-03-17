#include <cassert>
#include <iostream>
#include <sstream>
#include "../pixel.h"


// void test_bfs_chemin() {
//     std::istringstream in(pgm_2x2());
//     Graphe g(in);
//     // Prépare vecteur de prédécesseurs si BFS implémenté
//     std::vector<Pixel*> pred(g.L * g.C + 2, nullptr);
//     bool ok = g.chercherCheminAugmentant(pred);
//     // Si pas encore implémenté, commente cette ligne
//     assert(ok == true || ok == false); // Placeholder: affine quand BFS finalisé
// }

// // Test de getDeltaMin avec un chemin simple
// void test_getDeltaMin() {
//     //Initialisation d'un chemin simple
//     std::vector<Pixel*> pred;
//     Pixel p0, p1, p2;
//     p0.intensite = 0;
//     p1.intensite = 128;
//     p2.intensite = 255;
//     pred.push_back(nullptr); // source
//     pred.push_back(&p0);
//     pred.push_back(&p1);
//     pred.push_back(&p2); // puits

//     //test du delta calculé
//     int delta = g.getDeltaMin(pred);
//     assert(delta == 0);
// }

void run_all() {
    // test_bfs_chemin();
    test_getDeltaMin();
}

int main() {
    run_all();
    std::cout << "Tous les tests assert ont passé.\n";
    return 0;
}
