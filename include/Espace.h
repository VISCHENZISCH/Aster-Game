#ifndef ESPACE_H
#define ESPACE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "ElementEspace.h"


class Espace
{
	public:
		Espace();
		void ajouter(std::unique_ptr<ElementEspace> element);
		void actualiser();
		void gererCollisions();
		void afficher(sf::RenderWindow& fenetre) const;
		void nettoyer();
		void vider();
		inline bool espaceEstVide() const { return elements.empty() && aAjouter.empty();};

	protected:

	private:
		std::vector<std::unique_ptr<ElementEspace>> elements{};
		std::vector<std::unique_ptr<ElementEspace>> aAjouter{};
		sf::Clock chrono{};
};

#endif // ESPACE_H
