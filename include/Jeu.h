#ifndef JEU_H
#define JEU_H

#include "Espace.h"
#include <memory>
#include <exception>
#include <string>
#include <vector>
#include <SFML/Audio.hpp>

enum class EtatJeu {ACCUEIL, EN_COURS, PAUSE, GAME_OVER};
enum class TypeBonus {AUCUN, TIR_TRIPLE, BOUCLIER, TIR_RAPIDE};

class Jeu
{
	public:
		Jeu(Espace& p_espace);
		void demarrer();
		void terminer();
		void gererEvenement(sf::Event const& evenement);
		void actualiser(float temps);
		void ajouterPoints(int points, Coordonnees const& position);
		inline bool estEnCours() const { return etat == EtatJeu::EN_COURS; };
		inline bool estEnPause() const { return etat == EtatJeu::PAUSE; };
		inline bool demandeQuitter() const { return quitter; };
		void afficher(sf::RenderWindow& fenetre) const ;
		void initialiserExecption(std::exception const& exception);
		void gererVagues();
		void perdreVie();
		void declencherSecousse(float force);
		float getForceSecousse() const { return forceSecousse; }
		// Bonus
		void activerBonus(TypeBonus type);
		inline TypeBonus getBonusActif() const { return bonusActif; };
		inline sf::Font const& getPolice() const { return police; };
		
		static constexpr float DUREE_BONUS{8.f};
		
	private:
		struct TexteFlottant {
			sf::Text texte;
			float age{};
		};
		void basculerPause();
		void ajouterAsteroides();
		float getFacteurVitesse() const;
		sf::Text creerTexte(std::string const& contenu, unsigned taille, sf::Color const& couleur) const;
		void dessinerTexte(sf::RenderWindow& fenetre, sf::Text const& texte) const;
		void dessinerVoile(sf::RenderWindow& fenetre) const;
		void afficherHUD(sf::RenderWindow& fenetre) const;
		void afficherAccueil(sf::RenderWindow& fenetre) const;
		void afficherGameOver(sf::RenderWindow& fenetre) const;
		
		Espace& espace;
		EtatJeu etat{EtatJeu::ACCUEIL};
		bool quitter{false};
		int score{};
		int meilleurScore{};
		bool nouveauRecord{false};
		int vies{3};
		int vague{1};
		int prochaineVieBonus{10000};
		float forceSecousse{0.f};
		TypeBonus bonusActif{TypeBonus::AUCUN};
		float tempsBonusRestant{0.f};
		
		sf::Clock chronoClignotement{};
		sf::Clock chronoGameOver{};
		sf::Clock chronoAnnonce{};
		std::string message{};
		sf::Clock chronoMessage{};
		std::vector<TexteFlottant> textesFlottants{};
		
		sf::Sprite spriteAcceuil{};
		sf::Sprite spriteVie{};
		sf::Sound sonAccueil{};
		bool sonAccueilJoue {false};
		
		sf::Font police{};
		std::unique_ptr<sf::Text> textException{nullptr};
		
		static constexpr int POINTS_VIE_BONUS{10000};
		static constexpr float DUREE_ANNONCE{1.5f};
		static constexpr float DUREE_TEXTE_FLOTTANT{0.8f};
		static constexpr float DELAI_GAME_OVER{1.f};
};

#endif // JEU_H
