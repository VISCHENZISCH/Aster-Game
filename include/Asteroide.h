#ifndef ASTEROIDE_H
#define ASTEROIDE_H
#include "ElementEspace.h"
#include "Espace.h"
#include "Jeu.h"


class Asteroide : public ElementEspace
{
	public:
		explicit Asteroide(Jeu& p_jeu, Espace& p_espace, Asteroide* parent = nullptr, float p_facteurVitesse = 1.f);
		virtual void reagirCollision(TypeElement typeAutre) override;
		
	private:
		Jeu& jeu;
		Espace& espace;
		int taille{3};
		float facteurVitesse{1.f};
		static constexpr float CHANCE_BONUS{0.1f};
	
};

#endif // ASTEROIDE_H
