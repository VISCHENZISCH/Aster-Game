#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <ElementEspace.h>
#include <SFML/Audio.hpp>


class Explosion : public ElementEspace
{
	public:
		Explosion(Coordonnees const& p_position);
		virtual void reagirCollision(TypeElement typeAutre) override;
		
 
	protected:
		virtual void mettreAJour(float temps) override;

	private:
		float age{};
		sf::Sound son{};
		static constexpr float DUREE_VIE{0.1f};
};

#endif // EXPLOSION_H
