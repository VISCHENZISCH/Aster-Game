#ifndef VAISSEAU_H_INCLUDED
#define VAISSEAU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "Vecteur.h"
#include "Coordonnees.h"
#include "Explosion.h"
#include "Espace.h"
#include "Jeu.h"


class Vaisseau : public ElementEspace {
	
	public:
		
		explicit Vaisseau(Jeu& p_jeu, Espace& p_espace, sf::Color const& couleur);
		
		// Surchage d'une methode de la classe mère
		
		virtual void reagirCollision(TypeElement typeAutre) override;

	
	protected:
		virtual void mettreAJour(float temps) override;
		
	private:
		void actualiserEtat();
		bool accelerationEnCours{false};
		bool tourneAGauche{false};
		bool tourneADroite{false};
		
		Jeu& jeu;
		Espace& espace;
		sf::Clock dernierTir{};
		
		static constexpr float ACCELERATION{7000.f};
		static constexpr float COEFF_FROTTEMENTS{2.f};
		static constexpr float VITESSE_ANGULAIRE{100.f};
		
};


#endif 