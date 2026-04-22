#include "Jeu.h"

#include "Vaisseau.h"
#include "Asteroide.h"
#include <memory>
#include "GestionnaireRessources.h"
#include "font.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>


using namespace std::string_literals;


const sf::Color COULEUR_VAISSEAU{sf::Color{0, 255, 0}};
    
Jeu::Jeu(Espace& p_espace) : espace(p_espace)
{
	if(!police.loadFromMemory(Air_Americana_ttf, Air_Americana_ttf_size)) throw std::runtime_error("Police introuvable");
	textScore.setFont(police);
	textScore.move(50, 50);
	textMeilleurScore.setFont(police);
	textMeilleurScore.move(50,0);
	
	// Chargement du meilleur à partir du fichier meilleur_score.txt
	auto fichier = std::fstream{"meilleur_score.txt"};
	//fichier = std::fstream{"meilleur_score.txt", std::ios::in (écriture)  | std::ios::out (lecture) std::ios::app (ajout) ou ::tronc (suprrimer};
	if(fichier.is_open()) {
		fichier >>  meilleurScore;
	}
	fichier.close();
	
	rafraichirMeilleurScore();
	try {
		spriteAcceuil.setTexture(GestionnaireRessources<sf::Texture>::getRessources("ressources/acceuil.png"));
		sonAccueil.setBuffer(GestionnaireRessources<sf::SoundBuffer>::getRessources("ressources/winnerSound.wav"));
	 
	} catch (std::exception const& exception) {
		initialiserExecption(exception);
	}
	
}


// Démarrer le jeu 
void Jeu::demarrer() {
	enCours = true;
	score = 0;
	sonAccueilJoue = false;  // Réinitialiser pour rejouer le son au prochain game over
    sonAccueil.stop();        // Arrêt du  son d'accueil
	espace.ajouter(std::make_unique<Vaisseau>(*this, espace, COULEUR_VAISSEAU));
	espace.ajouter(std::make_unique<Asteroide>(*this, espace));
	espace.ajouter(std::make_unique<Asteroide>(*this, espace));
	espace.ajouter(std::make_unique<Asteroide>(*this, espace));
}

// Ajouter Poinst et Scores
void Jeu::ajouterPoints(int points) {
	score+=points;
	rafraichirText();

}
// Terminer le Jeu
void Jeu::terminer() {
	enCours = false;
	sonAccueilJoue = false;
	if(score > meilleurScore) {
		meilleurScore = score;
		rafraichirMeilleurScore();
		auto fichier = std::ofstream{"meilleur_score.txt"};
		if(fichier.is_open()) {
			fichier << meilleurScore;
		} else {
			throw std::runtime_error("Impossible  d'ouvrir le fichier meilleur_score en écriture");
		}
		fichier.close();
	}
	espace.vider();
	
}

// Afficher un acceuil pour le Jeu

void Jeu::afficher(sf::RenderWindow& fenetre) const {
	if(textException) {
		fenetre.draw(*textException);
	} else {
		if(!enCours && espace.espaceEstVide()) {
			fenetre.draw(spriteAcceuil);
			// Jouer le son une seule fois
			if(!sonAccueilJoue && sonAccueil.getStatus() != sf::Sound::Playing) {
				sonAccueilJoue = true;
				sonAccueil.play();
				
			}
		} else {
			fenetre.draw(textScore);
		}
		fenetre.draw(textMeilleurScore);
	}
}
// Initialiser Exception

void Jeu::initialiserExecption(std::exception const& exception) {
	textException = std::make_unique<sf::Text>();
	textException->setFont(police);
	textException->setString(exception.what());
	textException->setFillColor(sf::Color::Red);
}

// Rafraichir TextScore
 
 void Jeu::rafraichirText() {
 	textScore.setString("Score : "s + std::to_string(score));
 }
 
 // Rafraîchir le meilleur score 
 
 void Jeu::rafraichirMeilleurScore() {
 	textMeilleurScore.setString("Meilleur score : " + std::to_string(meilleurScore));
 }