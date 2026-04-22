#ifndef JEU_H
#define JEU_H

#include "Espace.h"
#include <memory>
#include <exception>
#include <SFML/Audio.hpp>

class Jeu
{
	public:
		Jeu(Espace& p_espace);
		void demarrer();
		void terminer();
		void ajouterPoints(int points);
		inline bool estEnCours() const { return enCours; };
		void afficher(sf::RenderWindow& fenetre) const ;
		void initialiserExecption(std::exception const& exception);
	protected:

	private:
		void rafraichirText();
		void rafraichirMeilleurScore();
		Espace& espace;
		bool enCours{false};
		int score{};
		int meilleurScore{};
		sf::Text textScore{};
		sf::Text textMeilleurScore{};
		sf::Sprite spriteAcceuil{};
		mutable sf::SoundBuffer buffer{};
		mutable sf::Sound sonAccueil{};
		mutable bool sonAccueilJoue {false};
		
		sf::Font police{};
		std::unique_ptr<sf::Text> textException{nullptr};
		
		
};

#endif // JEU_H
