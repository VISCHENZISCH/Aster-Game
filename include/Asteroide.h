#ifndef ASTEROIDE_H
#define ASTEROIDE_H
#include "ElementEspace.h"
#include "Espace.h"
#include "Jeu.h"


class Asteroide : public ElementEspace
{
	public:
		explicit Asteroide(Jeu& p_jeu, Espace& p_espace, Asteroide* parent = nullptr);
		virtual void reagirCollision(TypeElement typeAutre) override;
		
	private:
		Jeu& jeu;
		Espace& espace;
	
};

#endif // ASTEROIDE_H

