#include "Particule.h"
#include "Espace.h"
#include <iostream>
#include <random>

Particule::Particule(Coordonnees const& pos, Vecteur const& vit, sf::Color const& coul, float dureeVie) 
    : ElementEspace(""), dureeVieMax(dureeVie), dureeVieActuelle(dureeVie) {
    type = TypeElement::AUTRE;
    position = pos;
    vitesse = vit;
    forme.setRadius(2.0f);
    forme.setFillColor(coul);
    forme.setOrigin(1.0f, 1.0f);
}

void Particule::mettreAJour(float temps) {
    dureeVieActuelle -= temps;
    if (dureeVieActuelle <= 0) {
        estDetruit = true;
    } else {
        sf::Color c = forme.getFillColor();
        c.a = static_cast<sf::Uint8>(255.0f * (dureeVieActuelle / dureeVieMax));
        forme.setFillColor(c);
    }
}

void Particule::afficher(sf::RenderWindow& fenetre) const {
    // On copie la forme pour l'afficher (ElementEspace::afficher utilise le sprite, on le surcharge)
    sf::CircleShape affichageForme = forme;
    affichageForme.setPosition(position.getX(), position.getY());
    fenetre.draw(affichageForme);
}

void Particule::creerGerbe(Espace& espace, Coordonnees const& pos, sf::Color const& coul, int nombre) {
    static auto generateur = std::mt19937{std::random_device{}()};
    auto distributionAngle = std::uniform_real_distribution<float>{0.f, 360.f};
    auto distributionVitesse = std::uniform_real_distribution<float>{50.f, 300.f};
    auto distributionDuree = std::uniform_real_distribution<float>{0.3f, 0.9f};
    for (int i = 0; i < nombre; ++i) {
        auto vit = Vecteur::creerDepuisAngle(distributionVitesse(generateur), distributionAngle(generateur));
        espace.ajouter(std::make_unique<Particule>(pos, vit, coul, distributionDuree(generateur)));
    }
}
