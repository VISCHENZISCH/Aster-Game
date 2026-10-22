#ifndef PARTICULE_H
#define PARTICULE_H

#include "ElementEspace.h"
#include <SFML/Graphics.hpp>

class Espace;

class Particule : public ElementEspace {
public:
    Particule(Coordonnees const& pos, Vecteur const& vit, sf::Color const& coul, float dureeVie);
    virtual void reagirCollision(TypeElement typeAutre) override {};
    virtual void afficher(sf::RenderWindow& fenetre) const override;
    // Crée une gerbe de particules dans toutes les directions (explosions)
    static void creerGerbe(Espace& espace, Coordonnees const& pos, sf::Color const& coul, int nombre);
protected:
    virtual void mettreAJour(float temps) override;
private:
    float dureeVieMax;
    float dureeVieActuelle;
    sf::CircleShape forme;
};

#endif // PARTICULE_H
