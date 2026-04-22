#include "Coordonnees.h"
#include <cmath>
#include <iostream>
#include <algorithm>

int Coordonnees::longueurEspace = 0;
int Coordonnees::hauteurEspace = 0;
// Méthode de classe pour définir les limites de l'espace
void Coordonnees::initialiserEspace(float longueur, float hauteur) {
    if(longueurEspace != 0 || hauteurEspace != 0 ) {
        std::cerr << "Attention : l'espace était initialisé !" << std::endl;
    }
    longueurEspace = longueur;
    hauteurEspace = hauteur;
}

// Constructeur par défaut : position au milieu de l'espace
Coordonnees::Coordonnees() 
    : x(longueurEspace / 2.f), y(hauteurEspace / 2.f) {
}

// Constructeur avec paramètres
Coordonnees::Coordonnees(float px, float py) 
    : x(px), y(py) {
    normaliser(); // S'assurer que les coordonnées sont dans les limites
}

// Opérateur += pour ajouter un vecteur aux coordonnées
void Coordonnees::operator+=(Vecteur const& vecteur) {
    x += vecteur.x;
    y += vecteur.y;
    normaliser(); // S'assurer que les nouvelles coordonnées restent dans les limites
}

// Méthode privée pour normaliser les coordonnées (wrapping)
void Coordonnees::normaliser() {
    // Gestion du dépassement en x (wrapping horizontal)
    while (x < 0) {
        x += longueurEspace;
    }
    while (x >= longueurEspace) {
        x -= longueurEspace;
    }
    
    // Gestion du dépassement en y (wrapping vertical)
    while (y < 0) {
        y += hauteurEspace;
    }
    while (y >= hauteurEspace) {
        y -= hauteurEspace;
    }
}
float Coordonnees::calculerDistance(Coordonnees const& autre) {
    auto delta = Vecteur{std::min({ std::fabs(x-autre.x), std::fabs(x-autre.x-longueurEspace), std::fabs(x-autre.x+longueurEspace) }), 
                         std::min({ std::fabs(y-autre.y), std::fabs(y-autre.y-hauteurEspace), std::fabs(y-autre.y+hauteurEspace) })};
    return std::sqrt(delta.x * delta.x + delta.y * delta.y);
}