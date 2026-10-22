#include "ElementEspace.h"
#include <iostream>
#include <array>
#include "GestionnaireRessources.h"
#include <SFML/Graphics.hpp>


// Constructeur de classe et paramétrage 
ElementEspace::ElementEspace(std::string_view const& cheminImage){
	if (!cheminImage.empty()) {
		sprite.setTexture(GestionnaireRessources<sf::Texture>::getRessources(cheminImage));
		sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	}
	sprite.setPosition(position.getX(), position.getY());
	
}

// Mise à jour de la postion  dans le temps
void ElementEspace::actualiser(float temps) {
	mettreAJour(temps);
	auto deplacement = vitesse * temps;
	position += deplacement;
	sprite.setPosition(position.getX(), position.getY());
	sprite.rotate(vitesseAngulaire * temps);
}
void ElementEspace::mettreAJour(float temps) { };

// Affciher l'astéroide 
void ElementEspace::afficher(sf::RenderWindow& fenetre) const {
		auto affichages = std::array<Vecteur ,9>{ Vecteur{-1, -1}, Vecteur{0, -1}, Vecteur{1, -1},
												  Vecteur{-1, 0}, Vecteur{0, 0}, Vecteur{1, 0},
												  Vecteur{-1, 1}, Vecteur{0, 1}, Vecteur{1, 1}
											  };
											  
	for(auto& affichage : affichages) {
		auto transformation = sf::Transform{};
		transformation.translate(affichage.x * Coordonnees::getLongueurEspace(), affichage.y * Coordonnees::getHauteurEspace());
		fenetre.draw(sprite, transformation);
		
	}
	
}
float ElementEspace::getRayon() const {
	return sprite.getGlobalBounds().height/2.4f;
}


void ElementEspace::testerCollision(ElementEspace& autre) {
	auto distance = position.calculerDistance(autre.position);
	if(distance < getRayon() + autre.getRayon()) {
		reagirCollision(autre.type);
	}
}
