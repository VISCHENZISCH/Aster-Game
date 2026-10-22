#include "Asteroide.h"
#include <iostream>
#include <random>
#include "Explosion.h"
#include "Particule.h"
#include "Bonus.h"

// Constructeur de classe et paramétrage de l'astéroide
 Asteroide::Asteroide(Jeu& p_jeu, Espace& p_espace, Asteroide* parent, float p_facteurVitesse) : ElementEspace{"ressources/asteroide.png"},jeu{p_jeu}, espace{p_espace}, facteurVitesse{p_facteurVitesse} {
 	
 	type = TypeElement::ASTEROIDE;
 	auto generateur = std::random_device{};
 	auto distributionPositionX = std::uniform_real_distribution<float>{0.f, static_cast<float>(Coordonnees::getLongueurEspace())};
 	auto distributionPositionY = std::uniform_real_distribution<float>{100.f, 200.f}; // Apparaissent en HAUT, loin du vaisseau (même via le bord)
 	auto distributionVitesse = std::uniform_real_distribution<float>{100, 200};
 	auto distributionAngle = std::uniform_real_distribution<float>{70, 110}; // Tombent vers le BAS
 	auto distributionAngleFragment = std::uniform_real_distribution<float>{0, 360};
  	auto distributionVitesseAngulaire = std::uniform_real_distribution<float>{10, 30} ;
	vitesseAngulaire = distributionVitesseAngulaire(generateur);
	if(parent) {
		taille = parent->taille - 1;
		facteurVitesse = parent->facteurVitesse;
		sprite.setScale(parent->sprite.getScale().x/1.5f, parent->sprite.getScale().y/1.5f);
		position = parent->position;
		// Les fragments partent dans toutes les directions en gardant une partie de l'élan du parent
		vitesse = parent->vitesse * 0.5f + Vecteur::creerDepuisAngle(distributionVitesse(generateur) * facteurVitesse, distributionAngleFragment(generateur));
	} else {
		taille = 3;
		position = {distributionPositionX(generateur), distributionPositionY(generateur)};	
		vitesse = Vecteur::creerDepuisAngle(distributionVitesse(generateur) * facteurVitesse, distributionAngle(generateur));
	}
}

void Asteroide::reagirCollision(TypeElement typeAutre) {
	if(typeAutre == TypeElement::MISSILE && !estDetruit) {
		estDetruit = true;
		jeu.declencherSecousse(5.0f + taille * 2.0f);
		// Plus l'astéroïde est petit, plus il rapporte
		auto points = taille == 3 ? 20 : (taille == 2 ? 50 : 100);
		jeu.ajouterPoints(points, position);
		if(taille > 1) {
			espace.ajouter(std::make_unique<Asteroide>(jeu, espace, this));
			espace.ajouter(std::make_unique<Asteroide>(jeu, espace, this));
		}
		espace.ajouter(std::make_unique<Explosion>(position));
		Particule::creerGerbe(espace, position, sf::Color{255, 160, 60}, 8 + taille * 5);
		Particule::creerGerbe(espace, position, sf::Color{150, 140, 130}, 4 + taille * 3);
		if(rand() % 100 < CHANCE_BONUS * 100) {
			espace.ajouter(std::make_unique<Bonus>(jeu, position));
		}
	}
	
}
