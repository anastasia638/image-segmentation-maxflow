#include <cstdio>
#include <iostream>

int main(void)

{
    int i, j, n;

    for (i = 0; i < 11; i++) {
        for (j = 0; j < 10; j++) {
            n = 10*i + j; // Certaines valeurs de n correspondent à des couleurs
            if (n > 108) break;
            std::cout <<"\033[1;"<<n<<"m"<<" Couleur " << n << "\033[0m";
            // On récupère l'indice n de la couleur d'affichage puis on affiche n
        }
        std::cout << std::endl;
        
        /*
         for (i = 0; i < 11; i++) {
         for (j = 0; j < 10; j++) {
         n = 10*i + j; // Certaines valeurs de n correspondent à des couleurs
         if (n > 108) break;
         std::printf("\033[%dm Couleur %3d\033[m",n,n);
         // On récupère l'indice n de la couleur d'affichage puis on affiche n
         }
         printf("\n");
         }*/
    }
    
    return (0);
}
// Plus d'info sur les ANSI Escape Code : https://en.wikipedia.org/wiki/ANSI_escape_code

