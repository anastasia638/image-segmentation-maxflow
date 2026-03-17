#include "pixel.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include<cmath> 
#include<queue>
#include<vector>
#include<climits>

//Accesseurs simples pour une programmation modulaire
int Pixel::getIntensite() const {
    return intensite;
}
int Tuyau::getFlot() const {
    return flot;
}
int Tuyau::getCapacite() const {
    return capacite;
}

//Constucteur
Graphe::Graphe(const char* filename){

        //Chargement du fichier + vérification
        std::cout << "chargement de " << filename << "\n";
        std::ifstream image(filename);
        assert(image.good());

        //Lecture du fichier + vérification
        std::string tmp;
        image >> tmp;
        assert(tmp=="P2");

        // Calcul des lignes et colonnes
        int max;
        image >> C >> L >> max;
        std::cout<< C <<"colonne et"<<L<<"lignes\n";

        //Construction du tableau avec 2 indices en plus pour le puit et la source
        tabPixel = new Pixel[L*C+2];
        Pixel *source = &tabPixel[L * C];
        Pixel *puit   = &tabPixel[L * C + 1];
        source->intensite = 0;
        puit->intensite   = 0;

        //Choix des constantes
        float sigma = 9.0;
        float alpha = 16.0;

        //Initialisation des intensités
        for(int i=0;i<L*C;i++){
            image >> tabPixel[i].intensite;
        }

        //Remplissage du tableau
        for(int i=0;i<L*C;i++){
            Pixel& pixActuel = tabPixel[i];
            int ligne = i / C;
            int col   = i % C;

            // Initialisation des tuyaux au départ
            for (int k = 0; k < 6; ++k){
                pixActuel.voisins[k].entrée = &pixActuel;
                pixActuel.voisins[k].destination = nullptr;
                pixActuel.voisins[k].flot = 0;
                pixActuel.voisins[k].capacite = 0;
            }

            // Ouest
            if(col > 0){
                Tuyau &tuyauOuest =  pixActuel.voisins[0];
                tuyauOuest.entrée = &tabPixel[i];
                tuyauOuest.destination = &tabPixel[i - 1];
                int ip =tuyauOuest.entrée->getIntensite();
                int iq = tuyauOuest.destination->getIntensite();
                tuyauOuest.capacite = std::exp(-std::pow(ip - iq, 2) / (2 * std::pow(sigma, 2)));
            }
            // Est
            if(col < C - 1){
                Tuyau &tuyauEst =  pixActuel.voisins[1];
                tuyauEst.entrée = &tabPixel[i];
                tuyauEst.destination = &tabPixel[i + 1];
                int ip =tuyauEst.entrée->getIntensite();
                int iq = tuyauEst.destination->getIntensite();
                tuyauEst.capacite = std::exp(-std::pow(ip - iq, 2) / (2 * std::pow(sigma, 2)));
            }
            // Sud
            if(ligne < L - 1){
                Tuyau &tuyauSud =  pixActuel.voisins[2];
                tuyauSud.entrée = &tabPixel[i];
                tuyauSud.destination = &tabPixel[i + C];
                int ip =tuyauSud.entrée->getIntensite();
                int iq = tuyauSud.destination->getIntensite();
                tuyauSud.capacite = std::exp(-std::pow(ip - iq, 2) / (2 * std::pow(sigma, 2)));
            }
            // Nord
            if(ligne > 0){
                Tuyau &tuyauNord =  pixActuel.voisins[3];
                tuyauNord.entrée = &tabPixel[i];
                tuyauNord.destination = &tabPixel[i - C];
                int ip =tuyauNord.entrée->getIntensite();
                int iq = tuyauNord.destination->getIntensite();
                tuyauNord.capacite = std::exp(-std::pow(ip - iq, 2) / (2 * std::pow(sigma, 2)));
            }

            // Source
            {
                Tuyau &tuyauSource = pixActuel.voisins[4];
                tuyauSource.entrée = source;
                tuyauSource.destination = &pixActuel;
                double val = 0.0;
                if (pixActuel.getIntensite() > 0.0f)
                    val = - alpha * std::log(pixActuel.getIntensite() / 255.0);
                int cap = (int)std::lround(Graphe::H * val);
                if (cap < 0) cap = 0;
                tuyauSource.capacite = cap;
            }
            // Puit
            {
                Tuyau &tuyauPuit = pixActuel.voisins[5];
                tuyauPuit.destination = puit;
                double val = 0.0;
                if (pixActuel.getIntensite() < 255)
                    val = - alpha * std::log((255.0 - pixActuel.getIntensite()) / 255.0);
                int cap = (int)std::lround(Graphe::H * val);
                if (cap < 0) cap = 0;
                tuyauPuit.capacite = cap;
            }
        }
        //fin de lecture du fichier
        image.close();

    }
int Graphe::getIndex(int i, int j) const {
    return i * C + j;
}

// BFS pour trouver un chemin augmentant de Source vers Puits
bool Graphe::chercherCheminAugmentant(std::vector<Pixel*>& predecesseur) {
    Pixel* source = &tabPixel[L * C];
    Pixel* puit   = &tabPixel[L * C + 1];

    //vérification + réinitialisation du tableau des prédécesseurs
    if ((int)predecesseur.size() < L*C+2) predecesseur.assign(L*C+2, nullptr);
    else std::fill(predecesseur.begin(), predecesseur.end(), nullptr);

    std::vector<bool> visite(L * C + 2, false);
    std::queue<Pixel*> file;
    int idxSource = L*C, idxPuit = L*C+1;

    //On commence notre file par la source et on la marque comme visitée
    file.push(source);
    visite[idxSource] = true;

    //Tant qu'il y a des pixels à explorer
    while(!file.empty()){

        //On récupère le premier pixel de la file et on le supprime de la file
        Pixel* courant = file.front(); 
        file.pop();

        // CAS 1 : Tuyau Pixel -> source 
        if (courant == source) {
            for (int i = 0; i < L*C; ++i) {
                Tuyau& t = tabPixel[i].voisins[4];

                // Si le tuyau a de la capacité restante et que le pixel n'a pas été visité
                if (t.flot < t.capacite && !visite[i]) {
                    visite[i] = true; 
                    predecesseur[i] = courant;
                    file.push(&tabPixel[i]);
                }
            }
        }
        // CAS 2 : Tuyau pixel -> pixel + inverses
        else if (courant != source && courant != puit){
            for (int k = 0; k < 6; ++k) {
                Tuyau& t = courant->voisins[k];
                int idxV;

                //Pour les tuyaux inexistants (pixels en bordure) on passe
                if (!t.destination) 
                    continue;

                // Tuyau courant -> source
                else if (t.destination == source){
                    idxV = idxSource;
                }
                // Tuyau courant -> puit
                else if (t.destination == puit){
                    idxV = idxPuit;
                }
                // Tuyau courant -> pixel normal
                else {
                    idxV = int(t.destination - tabPixel);
                }
                //Si le tuyau a de la capacité restante
                if (t.flot < t.capacite && !visite[idxV]) {
                    visite[idxV] = true; 
                    predecesseur[idxV] = courant; 
                    file.push(t.destination);
                }
            }
        }
    }

        
    //retourne si le puit a été visité
    return visite[idxPuit];
}

// Trouve le delta minimum le long du chemin trouvé
int Graphe::getDeltaMin(const std::vector<Pixel*>& pred) {
    int deltaMin = INT_MAX;
    int idx;
    Pixel* source = &tabPixel[L*C];
    Pixel* puit   = &tabPixel[L*C+1];

    Pixel* courant = puit;
    // Remonte le chemin depuis le puit jusqu'à la source
    while (courant != source) {
        // on trouve l'index du pixel courant
        if(courant == puit)
            idx = L*C+1;
        else
            idx = int(courant - tabPixel);

        // on récupère le prédécesseur du pixel courant
        Pixel* pr = pred[idx];
        if (!pr) 
            return 0;

        // on vérifie les tuyaux entre pr et courant
        bool found = false;
        
        for (int k=0;k<6;k++){
            Tuyau& t = pr->voisins[k];
            if (t.destination == courant){
                deltaMin = std::min(deltaMin, t.getCapacite() - t.getFlot());
                found = true; 
                break;
            }
        }
        // Source -> pixel stocké dans courant->voisins[4]
        if (!found && pr == source){
            Tuyau& t = courant->voisins[4];
            if (t.entrée == pr){
                deltaMin = std::min(deltaMin, t.getCapacite() - t.getFlot());
                found = true;
            }
        }
        // arc inverse cur -> pr (réduction)
        if (!found){
            int best = INT_MAX;
            for (int k=0;k<6;k++){
                Tuyau& t = courant->voisins[k];
                if ((t.destination == pr || t.entrée == pr) && t.getFlot() > 0)
                    best = std::min(best, t.getFlot());
            }
            if (best != INT_MAX){ deltaMin = std::min(deltaMin, best); found = true; }
        }
        if (!found) return 0;
        courant = pr;
    }
    if (deltaMin == INT_MAX) {
        return 0;
    }
    return deltaMin;
}
// Augmente le flot le long du chemin trouvé de delta
void Graphe::augmenterFlot(const std::vector<Pixel*>& pred, int delta) {
    Pixel* source = &tabPixel[L*C];
    Pixel* puit   = &tabPixel[L*C+1];

    Pixel* courant = puit;
    while (courant != source){
        int idx = int(courant - tabPixel);
        if (courant == puit) {
            idx = L * C + 1;
        }
        Pixel* pr = pred[idx];
        assert(pr);
        bool updated=false;
        // pr -> courant
        for (int k=0;k<6;k++){
            Tuyau& t = pr->voisins[k];
            if (t.destination == courant){
                t.flot = std::min(t.getCapacite(), t.getFlot() + delta);
                updated=true; break;
            }
        }
        // Source -> pixel (dans courant->voisins[4])
        if (!updated && pr == source){
            Tuyau& t = courant->voisins[4];
            if (t.entrée == pr){
                t.flot = std::min(t.getCapacite(), t.getFlot() + delta);
                updated=true;
            }
        }
        // inverse: retirer du flot
        if (!updated){
            for (int k=0;k<6;k++){
                Tuyau& t = courant->voisins[k];
                if ((t.destination == pr || t.entrée == pr) && t.getFlot() > 0){
                    t.flot = std::max(0, t.getFlot() - delta);
                    updated=true; break;
                }
            }
        }
        assert(updated);
        courant = pr;
    }
}
// Binarisation de l'image selon les pixels atteignables depuis la source
void Graphe::BinariserImage() {
    Pixel* source = &tabPixel[L*C];
    Pixel* puit   = &tabPixel[L*C+1];

    std::vector<bool> vis(L*C+2,false);
    std::queue<Pixel*> file;
    int idxS = L*C, idxT = L*C+1;

    file.push(source); vis[idxS]=true;

    while(!file.empty()){
        Pixel* u=file.front(); 
        file.pop();

        if (u == source){
            for(int i=0;i<L*C;i++){
                Tuyau& t = tabPixel[i].voisins[4];
                if (t.destination && t.getFlot() < t.getCapacite() && !vis[i]){
                    vis[i]=true; file.push(&tabPixel[i]);
                }
            }
            continue;
        }
        for(int k=0;k<6;k++){
            Tuyau& t = u->voisins[k];
            if (!t.destination) continue;

            int idxV;
            if (t.destination == source) {
                idxV = idxS;
            } else if (t.destination == puit) {
                idxV = idxT;
            } else {
                idxV = int(t.destination - tabPixel);
            }
            if (t.getFlot() < t.getCapacite() && !vis[idxV]){
                vis[idxV]=true; file.push(t.destination);
            }

            Pixel* vInv = t.entrée;
            int idxInv;
            if (vInv == source) {
                idxInv = idxS;
            } else if (vInv == puit) {
                idxInv = idxT;
            } else {
                idxInv = int(vInv - tabPixel);
            }
            if (t.getFlot() > 0 && !vis[idxInv]){
                vis[idxInv]=true; file.push(vInv);
            }
        }
    }

    // Choix: atteignables depuis Source = 0 (noir), sinon 255 (blanc)
    for (int i = 0; i < L * C; ++i) {
        if (vis[i]) {
            tabPixel[i].intensite = 0;
        } else {
            tabPixel[i].intensite = 255;
        }
    }
}

void Graphe::sauvegarderImage(const char* filename) const {
    std::ofstream outFile(filename);
    assert(outFile.good());

    // Écrire l'en-tête PGM
    outFile << "P2\n";
    outFile << C << " " << L << "\n";
    outFile << "255\n";

    // Écrire les intensités des pixels
    for (int i = 0; i < L * C; ++i) {
        outFile << static_cast<int>(tabPixel[i].intensite) << " ";
        if ((i + 1) % C == 0) {
            outFile << "\n"; // Nouvelle ligne après chaque ligne de pixels
        }
    }

    outFile.close();
}


//Destructeur
Graphe::~Graphe(){
    delete[] tabPixel;
}
