#include <SFML/Graphics.hpp>
#include "Vaisseau.h"
#include "Missile.h"
#include "Particule.h"
#include <iostream>
#include <memory>


Vaisseau::Vaisseau(Jeu& p_jeu, Espace& p_espace, sf::Color const& couleur) : ElementEspace("ressources/vaisseau.png"), jeu{p_jeu}, espace{p_espace}, couleurBase{couleur} {
	type = TypeElement::VAISSEAU;
	sprite.setColor(couleurBase);
	// L'image pointe vers la droite (angle 0) : on tourne le vaisseau vers le haut, positionné en bas
	sprite.setRotation(-90.f);
	position = Coordonnees(Coordonnees::getLongueurEspace() / 2.0f, Coordonnees::getHauteurEspace() - 80.0f);
}


void Vaisseau::actualiserEtat() {
	accelerationEnCours = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	tourneAGauche = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	tourneADroite = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	auto cadence = jeu.getBonusActif() == TypeBonus::TIR_RAPIDE ? CADENCE_TIR_RAPIDE : CADENCE_TIR;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && dernierTir.getElapsedTime().asSeconds() > cadence) {
		tirer();
		dernierTir.restart();
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && chronoHyperspace.getElapsedTime().asSeconds() > 2.0f) {
		position = Coordonnees(static_cast<float>(rand() % Coordonnees::getLongueurEspace()), static_cast<float>(rand() % Coordonnees::getHauteurEspace()));
		vitesse = Vecteur{0, 0};
		chronoHyperspace.restart();
	}
		
}

// Tir depuis le nez du vaisseau dans sa direction, en éventail avec le bonus tir triple
void Vaisseau::tirer() {
	auto direction = sprite.getRotation();
	auto nez = position;
	nez += Vecteur::creerDepuisAngle(getRayon(), direction);
	espace.ajouter(std::make_unique<Missile>(nez, direction));
	if(jeu.getBonusActif() == TypeBonus::TIR_TRIPLE) {
		espace.ajouter(std::make_unique<Missile>(nez, direction + 15.f));
		espace.ajouter(std::make_unique<Missile>(nez, direction - 15.f));
	}
}

void Vaisseau::mettreAJour(float temps) {
	actualiserEtat(); 
	if (!estDetruit) {
		if (chronoInvincibilite.getElapsedTime().asSeconds() < DUREE_INVINCIBILITE) {
			if (chronoInvincibilite.getElapsedTime().asMilliseconds() % 200 < 100) {
				sprite.setColor(sf::Color::Transparent);
			} else {
				sprite.setColor(couleurBase);
			}
		} else {
			sprite.setColor(couleurBase);
		}

		// Rotation sur place
		vitesseAngulaire = 0.f;
		if (tourneAGauche) vitesseAngulaire = -VITESSE_ANGULAIRE;
		if (tourneADroite) vitesseAngulaire = VITESSE_ANGULAIRE;
		
		// Poussée dans la direction du vaisseau (marche arrière plus faible), avec inertie
		auto direction = sprite.getRotation();
		if (accelerationEnCours) {
			vitesse += Vecteur::creerDepuisAngle(ACCELERATION * temps, direction);
			if (rand() % 2 == 0) {
				auto arriere = position;
				arriere += Vecteur::creerDepuisAngle(getRayon(), direction + 180.f);
				Vecteur jet = Vecteur::creerDepuisAngle(200.f, direction + 180.f + (rand() % 30 - 15));
				espace.ajouter(std::make_unique<Particule>(arriere, vitesse + jet, sf::Color(255, 150, 0), 0.4f));
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			vitesse -= Vecteur::creerDepuisAngle(ACCELERATION * 0.5f * temps, direction);
		}
		vitesse -= vitesse * COEFF_FROTTEMENTS * temps;
	}

}

// Réagir à une collison
void Vaisseau::reagirCollision(TypeElement typeAutre) {
	if(estDetruit || typeAutre != TypeElement::ASTEROIDE) return;
	if(jeu.getBonusActif() == TypeBonus::BOUCLIER) return;
	if(chronoInvincibilite.getElapsedTime().asSeconds() >= DUREE_INVINCIBILITE) {
		estDetruit = true;
		jeu.declencherSecousse(20.f);
		espace.ajouter(std::make_unique<Explosion>(position));
		Particule::creerGerbe(espace, position, couleurBase, 30);
		jeu.perdreVie();
	}
}

// Afficher le vaisseau et son bouclier éventuel
void Vaisseau::afficher(sf::RenderWindow& fenetre) const {
	ElementEspace::afficher(fenetre);
	if(jeu.getBonusActif() == TypeBonus::BOUCLIER) {
		auto bouclier = sf::CircleShape{getRayon() + 18.f};
		bouclier.setOrigin(bouclier.getRadius(), bouclier.getRadius());
		bouclier.setPosition(position.getX(), position.getY());
		bouclier.setFillColor(sf::Color{0, 255, 255, 40});
		bouclier.setOutlineColor(sf::Color::Cyan);
		bouclier.setOutlineThickness(2.f);
		fenetre.draw(bouclier);
	}
}

