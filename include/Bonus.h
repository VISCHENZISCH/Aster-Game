#ifndef BONUS_H
#define BONUS_H

#include "ElementEspace.h"
#include "Jeu.h"
#include <SFML/Graphics.hpp>
#include <string>


class Bonus : public ElementEspace
{
	public:
		Bonus(Jeu& p_jeu, Coordonnees const& p_position);
		virtual void reagirCollision(TypeElement typeAutre) override;
		virtual void afficher(sf::RenderWindow& fenetre) const override;
		virtual float getRayon() const override { return RAYON; };
		static sf::Color getCouleur(TypeBonus type);
		static std::string getNom(TypeBonus type);

	protected:
		virtual void mettreAJour(float temps) override;

	private:
		Jeu& jeu;
		TypeBonus typeBonus{TypeBonus::TIR_TRIPLE};
		float age{};
		sf::CircleShape forme{};
		sf::Text lettre{};
		static constexpr float RAYON{18.f};
		static constexpr float DUREE_VIE{8.f};
};

#endif // BONUS_H
