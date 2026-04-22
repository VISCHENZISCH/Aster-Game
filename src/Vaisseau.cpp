#include <SFML/Graphics.hpp>
#include "Vaisseau.h"
#include "Missile.h"
#include <iostream>
#include <memory>


Vaisseau::Vaisseau(Jeu& p_jeu, Espace& p_espace, sf::Color const& couleur) : ElementEspace("ressources/vaisseau.png"), jeu{p_jeu}, espace{p_espace} {
	type = TypeElement::VAISSEAU;
	sprite.setColor(couleur);
	
}


void Vaisseau::actualiserEtat() {
	accelerationEnCours = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	tourneAGauche = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	tourneADroite = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && dernierTir.getElapsedTime().asSeconds() > 0.1) {
		espace.ajouter(std::make_unique<Missile>(position, sprite.getRotation()));
		dernierTir.restart();
	}
		
}

void Vaisseau::mettreAJour(float temps) {
	actualiserEtat(); 
	if (!estDetruit) {
		if (accelerationEnCours) {
			vitesse += Vecteur::creerDepuisAngle(ACCELERATION * temps, sprite.getRotation());
		}
		vitesse -= vitesse * COEFF_FROTTEMENTS * temps;
		
		if(tourneAGauche) {
			vitesseAngulaire = - VITESSE_ANGULAIRE;
		} else if (tourneADroite) {
			vitesseAngulaire = VITESSE_ANGULAIRE;
		} else {
			vitesseAngulaire = 0 ;
		}
	}

}

// Réagir à une collison
void Vaisseau::reagirCollision(TypeElement typeAutre) {
	if(typeAutre == TypeElement::ASTEROIDE) {
		estDetruit = true;
		jeu.terminer();
		espace.ajouter(std::make_unique<Explosion>(position));
	}
}

