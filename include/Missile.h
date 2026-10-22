#ifndef MISSILE_H
#define MISSILE_H

#include <ElementEspace.h>
#include <SFML/Audio.hpp>


class Missile : public ElementEspace
{
	public:
		Missile(Coordonnees const& p_position, float rotation);
		virtual void reagirCollision(TypeElement typeAutre) override;

	protected:
		virtual void mettreAJour(float temps) override;

	private:
		float age{};
		sf::Sound son{};
		static constexpr float VITESSE{2000.f};
		// Durée de vie : environ une hauteur d'écran parcourue
		static constexpr float DUREE_VIE{0.45f};
};

#endif // MISSILE_H
