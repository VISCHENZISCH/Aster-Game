#include "Bonus.h"
#include <cmath>

Bonus::Bonus(Jeu& p_jeu, Coordonnees const& p_position) : ElementEspace(""), jeu{p_jeu}
{
	type = TypeElement::BONUS;
	position = p_position;
	vitesse = Vecteur{0.f, 60.f}; // Descend doucement vers le joueur
	typeBonus = static_cast<TypeBonus>(1 + rand() % 3);
	
	auto couleur = getCouleur(typeBonus);
	forme.setRadius(RAYON);
	forme.setOrigin(RAYON, RAYON);
	forme.setFillColor(sf::Color{couleur.r, couleur.g, couleur.b, 60});
	forme.setOutlineColor(couleur);
	forme.setOutlineThickness(3.f);
	
	lettre.setFont(jeu.getPolice());
	switch(typeBonus) {
		case TypeBonus::TIR_TRIPLE: lettre.setString("3"); break;
		case TypeBonus::BOUCLIER: lettre.setString("B"); break;
		default: lettre.setString("R"); break;
	}
	lettre.setCharacterSize(22);
	lettre.setFillColor(sf::Color::White);
	auto limites = lettre.getLocalBounds();
	lettre.setOrigin(limites.left + limites.width / 2.f, limites.top + limites.height / 2.f);
}

void Bonus::mettreAJour(float temps) {
	age += temps;
	if(age > DUREE_VIE) {
		estDetruit = true;
	}
}

void Bonus::reagirCollision(TypeElement typeAutre) {
	if(typeAutre == TypeElement::VAISSEAU && !estDetruit) {
		estDetruit = true;
		jeu.activerBonus(typeBonus);
	}
}

void Bonus::afficher(sf::RenderWindow& fenetre) const {
	// Clignote pendant les 2 dernières secondes
	if(age > DUREE_VIE - 2.f && std::fmod(age, 0.25f) < 0.12f) return;
	
	// Légère pulsation pour attirer l'oeil
	auto echelle = 1.f + 0.1f * std::sin(age * 6.f);
	auto transformation = sf::Transform{};
	transformation.translate(position.getX(), position.getY());
	transformation.scale(echelle, echelle);
	fenetre.draw(forme, transformation);
	fenetre.draw(lettre, transformation);
}

sf::Color Bonus::getCouleur(TypeBonus type) {
	switch(type) {
		case TypeBonus::TIR_TRIPLE: return sf::Color{255, 140, 0};
		case TypeBonus::BOUCLIER: return sf::Color::Cyan;
		case TypeBonus::TIR_RAPIDE: return sf::Color::Yellow;
		default: return sf::Color::White;
	}
}

std::string Bonus::getNom(TypeBonus type) {
	switch(type) {
		case TypeBonus::TIR_TRIPLE: return "TIR TRIPLE";
		case TypeBonus::BOUCLIER: return "BOUCLIER";
		case TypeBonus::TIR_RAPIDE: return "TIR RAPIDE";
		default: return "";
	}
}
