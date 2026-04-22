#ifndef COORDONNEES_H
#define COORDONNEES_H
#include "../include/Vecteur.h"


class Coordonnees {
public:
    Coordonnees();
    Coordonnees(float px, float py);
    // Méthode de classe pour initialiser l'espace
    static void initialiserEspace(float longueur, float hauteur);
    // Constructeur par défaut (milieu de l'espace)
    void operator+=(Vecteur const& vecteur);
    // Constructeur avec paramètres
    void normaliser();
    // Getters
    inline float getX() const {return x; };
    inline float getY() const {return y; };
    // Getters statiques pour les dimensions de l'espace :  inline pour plus de performances
    inline static int getLongueurEspace() { return longueurEspace; } ;
    inline static int getHauteurEspace(){ return hauteurEspace; } ;
    float calculerDistance(Coordonnees const& autre);

private:
    float x;
    float y;
    
    // Méthode privée pour normaliser les coordonnées
    static int longueurEspace;
    static int hauteurEspace;

};

#endif // COORDONNEES_H