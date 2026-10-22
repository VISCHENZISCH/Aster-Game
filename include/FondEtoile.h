#ifndef FONDETOILE_H
#define FONDETOILE_H

#include <SFML/Graphics.hpp>
#include <vector>


// Champ d'étoiles qui défile lentement vers le bas (deux couches pour un effet de profondeur)
class FondEtoile
{
	public:
		explicit FondEtoile(int nombre);
		void actualiser(float temps);
		void afficher(sf::RenderWindow& fenetre) const;

	private:
		struct Etoile {
			sf::Vector2f position;
			float vitesse;
			float taille;
			sf::Color couleur;
		};
		std::vector<Etoile> etoiles{};
		sf::VertexArray sommets{sf::Quads};
};

#endif // FONDETOILE_H
