// LIFAP6 - Automne 2017 - R. Chaine

#ifndef _PIXEL
#define _PIXEL
#include <vector>  

struct Pixel; 
struct Tuyau{
    Pixel *entrée;
    Pixel *destination;
    int flot;
    int capacite;   

    //Accesseurs simples
    int getFlot() const;
    int getCapacite() const;
    
};

struct Pixel{
    int intensite;
    Tuyau voisins[6];

    //Accesseurs simples
    int getIntensite() const;

};



struct Graphe{
    Pixel *tabPixel;
    int L,C;
    
    //Constructeur
    Graphe(const char* filename);

    // Constantes
    static const int H = 100;

    //Méthodes
    // Prends des indices i,j et renvoie l'index dans le tableau
    int getIndex(int i, int j) const;
    // Prends un vecteur de Pixels, Résultat attendu: true ssi un chemin augmentant a été trouvé
    bool chercherCheminAugmentant(std::vector<Pixel*>& predecesseurs);
    // Renvoie le delta minimum le long du chemin augmentant
    int  getDeltaMin(const std::vector<Pixel*>& predecesseurs);
    // Prends un vecteur de Pixels et un delta, augmente le flot le long du chemin augmentant
    void augmenterFlot(const std::vector<Pixel*>& predecesseurs, int delta);
    // Prend un vecteur de Pixels et binarise les intensités du graphe
    void BinariserImage();
    // Prend un nom de fichier et sauvegarde l’image binaire avec ce nom
    void sauvegarderImage(const char* filename) const;

    //Destructeur
    ~Graphe();
};

#endif
