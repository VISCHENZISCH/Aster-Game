#include "FondEtoile.h"
#include "Coordonnees.h"
#include <random>

FondEtoile::FondEtoile(int nombre)
{
	auto generateur = std::mt19937{std::random_device{}()};
	auto distributionX = std::uniform_real_distribution<float>{0.f, static_cast<float>(Coordonnees::getLongueurEspace())};
	auto distributionY = std::uniform_real_distribution<float>{0.f, static_cast<float>(Coordonnees::getHauteurEspace())};
	auto distributionLuminosite = std::uniform_int_distribution<int>{80, 255};
	for(int i = 0; i < nombre; ++i) {
		// Un tiers d'étoiles proches : plus grosses et plus rapides
		auto proche = i % 3 == 0;
		auto luminosite = static_cast<sf::Uint8>(distributionLuminosite(generateur));
		etoiles.push_back({{distributionX(generateur), distributionY(generateur)},
		                   proche ? 40.f : 15.f,
		                   proche ? 2.f : 1.f,
		                   sf::Color{luminosite, luminosite, static_cast<sf::Uint8>(std::min(255, luminosite + 30))}});
	}
	sommets.resize(etoiles.size() * 4);
}

void FondEtoile::actualiser(float temps) {
	auto const hauteur = static_cast<float>(Coordonnees::getHauteurEspace());
	for(auto i{0u}; i < etoiles.size(); ++i) {
		auto& etoile = etoiles[i];
		etoile.position.y += etoile.vitesse * temps;
		if(etoile.position.y > hauteur) {
			etoile.position.y -= hauteur;
		}
		auto const& p = etoile.position;
		auto t = etoile.taille;
		sommets[i * 4 + 0] = sf::Vertex{{p.x, p.y}, etoile.couleur};
		sommets[i * 4 + 1] = sf::Vertex{{p.x + t, p.y}, etoile.couleur};
		sommets[i * 4 + 2] = sf::Vertex{{p.x + t, p.y + t}, etoile.couleur};
		sommets[i * 4 + 3] = sf::Vertex{{p.x, p.y + t}, etoile.couleur};
	}
}

void FondEtoile::afficher(sf::RenderWindow& fenetre) const {
	fenetre.draw(sommets);
}
