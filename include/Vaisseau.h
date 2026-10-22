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
		virtual void afficher(sf::RenderWindow& fenetre) const override;

	
	protected:
		virtual void mettreAJour(float temps) override;
		
	private:
		void actualiserEtat();
		void tirer();
		bool accelerationEnCours{false};
		bool tourneAGauche{false};
		bool tourneADroite{false};
		
		Jeu& jeu;
		Espace& espace;
		sf::Clock dernierTir{};
		sf::Clock chronoInvincibilite{};
		sf::Clock chronoHyperspace{};
		sf::Color couleurBase;
		
		static constexpr float ACCELERATION{1200.f};
		static constexpr float COEFF_FROTTEMENTS{1.5f}; // Vitesse max = ACCELERATION / COEFF_FROTTEMENTS
		static constexpr float VITESSE_ANGULAIRE{250.f};
		static constexpr float CADENCE_TIR{0.1f};
		static constexpr float CADENCE_TIR_RAPIDE{0.05f};
		static constexpr float DUREE_INVINCIBILITE{2.5f};
		
};


#endif 