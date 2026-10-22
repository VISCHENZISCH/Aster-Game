#include "Jeu.h"

#include "Vaisseau.h"
#include "Asteroide.h"
#include "Bonus.h"
#include <memory>
#include <algorithm>
#include "GestionnaireRessources.h"
#include "font.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>


using namespace std::string_literals;


const sf::Color COULEUR_VAISSEAU{sf::Color{0, 255, 0}};
const sf::Color COULEUR_TITRE{sf::Color{255, 190, 60}};

// Place l'origine du texte en son centre puis le positionne
static void centrer(sf::Text& texte, float x, float y) {
	auto limites = texte.getLocalBounds();
	texte.setOrigin(limites.left + limites.width / 2.f, limites.top + limites.height / 2.f);
	texte.setPosition(x, y);
}

// Applique une transparence (0 à 1) au texte
static void appliquerOpacite(sf::Text& texte, float opacite) {
	auto couleur = texte.getFillColor();
	couleur.a = static_cast<sf::Uint8>(255.f * std::clamp(opacite, 0.f, 1.f));
	texte.setFillColor(couleur);
}
    
Jeu::Jeu(Espace& p_espace) : espace(p_espace)
{
	if(!police.loadFromMemory(Air_Americana_ttf, Air_Americana_ttf_size)) throw std::runtime_error("Police introuvable");
	
	// Chargement du meilleur à partir du fichier meilleur_score.txt
	auto fichier = std::fstream{"meilleur_score.txt"};
	if(fichier.is_open()) {
		fichier >>  meilleurScore;
	}
	fichier.close();
	
	try {
		spriteAcceuil.setTexture(GestionnaireRessources<sf::Texture>::getRessources("ressources/acceuil.png"));
		sonAccueil.setBuffer(GestionnaireRessources<sf::SoundBuffer>::getRessources("ressources/winnerSound.wav"));
		spriteVie.setTexture(GestionnaireRessources<sf::Texture>::getRessources("ressources/vaisseau.png"));
		spriteVie.setOrigin(spriteVie.getLocalBounds().width / 2, spriteVie.getLocalBounds().height / 2);
		spriteVie.setScale(0.5f, 0.5f);
		spriteVie.setRotation(-90.f);
		spriteVie.setColor(COULEUR_VAISSEAU);
	} catch (std::exception const& exception) {
		initialiserExecption(exception);
	}
	
}


// Démarrer le jeu 
void Jeu::demarrer() {
	espace.vider();
	etat = EtatJeu::EN_COURS;
	score = 0;
	vies = 3;
	vague = 1;
	prochaineVieBonus = POINTS_VIE_BONUS;
	nouveauRecord = false;
	bonusActif = TypeBonus::AUCUN;
	tempsBonusRestant = 0.f;
	textesFlottants.clear();
	message.clear();
    sonAccueil.stop();        // Arrêt du  son d'accueil
	espace.ajouter(std::make_unique<Vaisseau>(*this, espace, COULEUR_VAISSEAU));
	ajouterAsteroides();
	chronoAnnonce.restart();
}

// Gestion du clavier selon l'état du jeu
void Jeu::gererEvenement(sf::Event const& evenement) {
	if(evenement.type != sf::Event::KeyPressed) return;
	auto touche = evenement.key.code;
	switch(etat) {
		case EtatJeu::ACCUEIL:
			if(touche == sf::Keyboard::Escape) quitter = true;
			else demarrer();
			break;
		case EtatJeu::EN_COURS:
			if(touche == sf::Keyboard::P || touche == sf::Keyboard::Escape) basculerPause();
			break;
		case EtatJeu::PAUSE:
			if(touche == sf::Keyboard::P || touche == sf::Keyboard::Escape || touche == sf::Keyboard::Enter) basculerPause();
			break;
		case EtatJeu::GAME_OVER:
			// Petit délai pour éviter de relancer par erreur en tirant
			if(chronoGameOver.getElapsedTime().asSeconds() < DELAI_GAME_OVER) break;
			if(touche == sf::Keyboard::Enter) {
				demarrer();
			} else if(touche == sf::Keyboard::Escape) {
				espace.vider();
				etat = EtatJeu::ACCUEIL;
			}
			break;
	}
}

void Jeu::basculerPause() {
	if(etat == EtatJeu::EN_COURS) {
		etat = EtatJeu::PAUSE;
	} else if(etat == EtatJeu::PAUSE) {
		etat = EtatJeu::EN_COURS;
		espace.reinitialiserChrono(); // Pas de saut de temps à la reprise
	}
}

// Mise à jour des éléments propres au jeu (secousse, bonus, textes...)
void Jeu::actualiser(float temps) {
	forceSecousse = std::max(0.f, forceSecousse - temps * 50.f);
	
	if((etat == EtatJeu::ACCUEIL || etat == EtatJeu::GAME_OVER) && !sonAccueilJoue) {
		sonAccueilJoue = true;
		sonAccueil.play();
	}
	
	if(etat == EtatJeu::PAUSE) return;
	
	if(bonusActif != TypeBonus::AUCUN) {
		tempsBonusRestant -= temps;
		if(tempsBonusRestant <= 0.f) {
			bonusActif = TypeBonus::AUCUN;
		}
	}
	
	for(auto& flottant : textesFlottants) {
		flottant.age += temps;
		flottant.texte.move(0.f, -60.f * temps);
		appliquerOpacite(flottant.texte, 1.f - flottant.age / DUREE_TEXTE_FLOTTANT);
	}
	textesFlottants.erase(std::remove_if(std::begin(textesFlottants), std::end(textesFlottants),
		[](TexteFlottant const& flottant) { return flottant.age > DUREE_TEXTE_FLOTTANT; }), std::end(textesFlottants));
}

// Ajouter Points et Scores
void Jeu::ajouterPoints(int points, Coordonnees const& position) {
	if(!estEnCours()) return;
	score += points;
	
	auto flottant = TexteFlottant{creerTexte("+"s + std::to_string(points), 22, sf::Color::Yellow)};
	centrer(flottant.texte, position.getX(), position.getY());
	textesFlottants.push_back(flottant);
	
	while(score >= prochaineVieBonus) {
		vies++;
		prochaineVieBonus += POINTS_VIE_BONUS;
		message = "VIE BONUS !";
		chronoMessage.restart();
	}
}

// Terminer le Jeu
void Jeu::terminer() {
	etat = EtatJeu::GAME_OVER;
	chronoGameOver.restart();
	sonAccueilJoue = false;
	bonusActif = TypeBonus::AUCUN;
	if(score > meilleurScore) {
		meilleurScore = score;
		nouveauRecord = true;
		auto fichier = std::ofstream{"meilleur_score.txt"};
		if(fichier.is_open()) {
			fichier << meilleurScore;
		} else {
			throw std::runtime_error("Impossible  d'ouvrir le fichier meilleur_score en écriture");
		}
		fichier.close();
	}
}

// Afficher l'interface selon l'état du jeu
void Jeu::afficher(sf::RenderWindow& fenetre) const {
	if(textException) {
		fenetre.draw(*textException);
		return;
	}
	switch(etat) {
		case EtatJeu::ACCUEIL:
			afficherAccueil(fenetre);
			break;
		case EtatJeu::EN_COURS:
			afficherHUD(fenetre);
			break;
		case EtatJeu::PAUSE: {
			afficherHUD(fenetre);
			dessinerVoile(fenetre);
			auto titre = creerTexte("PAUSE", 80, COULEUR_TITRE);
			centrer(titre, Coordonnees::getLongueurEspace() / 2.f, Coordonnees::getHauteurEspace() / 2.f - 30.f);
			dessinerTexte(fenetre, titre);
			auto aide = creerTexte("P OU ECHAP POUR REPRENDRE", 26, sf::Color::White);
			centrer(aide, Coordonnees::getLongueurEspace() / 2.f, Coordonnees::getHauteurEspace() / 2.f + 50.f);
			dessinerTexte(fenetre, aide);
			break;
		}
		case EtatJeu::GAME_OVER:
			afficherGameOver(fenetre);
			break;
	}
}

void Jeu::afficherAccueil(sf::RenderWindow& fenetre) const {
	fenetre.draw(spriteAcceuil);
	
	auto bandeau = sf::RectangleShape{{static_cast<float>(Coordonnees::getLongueurEspace()), 50.f}};
	bandeau.setPosition(0.f, Coordonnees::getHauteurEspace() - 50.f);
	bandeau.setFillColor(sf::Color{0, 0, 0, 170});
	fenetre.draw(bandeau);
	
	auto commandes = creerTexte("GAUCHE / DROITE : TOURNER     HAUT : AVANCER     BAS : RECULER     ESPACE : TIRER     MAJ : HYPERESPACE     P : PAUSE     ECHAP : QUITTER", 18, sf::Color{200, 200, 200});
	centrer(commandes, Coordonnees::getLongueurEspace() / 2.f, Coordonnees::getHauteurEspace() - 25.f);
	fenetre.draw(commandes);
	
	auto record = creerTexte("MEILLEUR SCORE : "s + std::to_string(meilleurScore), 22, COULEUR_TITRE);
	record.setPosition(Coordonnees::getLongueurEspace() - record.getLocalBounds().width - 30.f, 20.f);
	dessinerTexte(fenetre, record);
}

void Jeu::afficherHUD(sf::RenderWindow& fenetre) const {
	auto const longueur = static_cast<float>(Coordonnees::getLongueurEspace());
	auto const hauteur = static_cast<float>(Coordonnees::getHauteurEspace());
	
	// Score centré en haut, meilleur score à droite
	auto textScore = creerTexte(std::to_string(score), 40, sf::Color::White);
	centrer(textScore, longueur / 2.f, 40.f);
	dessinerTexte(fenetre, textScore);
	
	auto textRecord = creerTexte("RECORD : "s + std::to_string(std::max(score, meilleurScore)), 20, COULEUR_TITRE);
	textRecord.setPosition(longueur - textRecord.getLocalBounds().width - 30.f, 25.f);
	dessinerTexte(fenetre, textRecord);
	
	// Vies sous forme d'icônes du vaisseau
	auto icone = spriteVie;
	for(int i = 0; i < vies; ++i) {
		icone.setPosition(50.f + i * 45.f, 40.f);
		fenetre.draw(icone);
	}
	
	auto textVague = creerTexte("VAGUE "s + std::to_string(vague), 20, sf::Color::Cyan);
	textVague.setPosition(30.f, 70.f);
	dessinerTexte(fenetre, textVague);
	
	// Bonus actif et sa jauge de durée en bas à gauche
	if(bonusActif != TypeBonus::AUCUN) {
		auto couleur = Bonus::getCouleur(bonusActif);
		auto textBonus = creerTexte(Bonus::getNom(bonusActif), 20, couleur);
		textBonus.setPosition(30.f, hauteur - 60.f);
		dessinerTexte(fenetre, textBonus);
		
		auto fond = sf::RectangleShape{{200.f, 8.f}};
		fond.setPosition(30.f, hauteur - 30.f);
		fond.setFillColor(sf::Color{255, 255, 255, 50});
		fenetre.draw(fond);
		auto jauge = sf::RectangleShape{{200.f * tempsBonusRestant / DUREE_BONUS, 8.f}};
		jauge.setPosition(30.f, hauteur - 30.f);
		jauge.setFillColor(couleur);
		fenetre.draw(jauge);
	}
	
	// Annonce de la vague au centre
	auto tempsAnnonce = chronoAnnonce.getElapsedTime().asSeconds();
	if(tempsAnnonce < DUREE_ANNONCE) {
		auto annonce = creerTexte("VAGUE "s + std::to_string(vague), 72, COULEUR_TITRE);
		centrer(annonce, longueur / 2.f, hauteur / 2.f - 60.f);
		appliquerOpacite(annonce, 1.f - tempsAnnonce / DUREE_ANNONCE);
		dessinerTexte(fenetre, annonce);
	}
	
	// Message temporaire (vie bonus, power-up)
	auto tempsMessage = chronoMessage.getElapsedTime().asSeconds();
	if(!message.empty() && tempsMessage < DUREE_ANNONCE) {
		auto textMessage = creerTexte(message, 36, sf::Color::Green);
		centrer(textMessage, longueur / 2.f, hauteur / 2.f + 20.f);
		appliquerOpacite(textMessage, 1.f - tempsMessage / DUREE_ANNONCE);
		dessinerTexte(fenetre, textMessage);
	}
	
	for(auto const& flottant : textesFlottants) {
		dessinerTexte(fenetre, flottant.texte);
	}
}

void Jeu::afficherGameOver(sf::RenderWindow& fenetre) const {
	auto const centreX = Coordonnees::getLongueurEspace() / 2.f;
	auto const centreY = Coordonnees::getHauteurEspace() / 2.f;
	dessinerVoile(fenetre);
	
	auto titre = creerTexte("GAME OVER", 90, sf::Color{230, 60, 60});
	centrer(titre, centreX, centreY - 130.f);
	dessinerTexte(fenetre, titre);
	
	auto textScore = creerTexte("SCORE : "s + std::to_string(score) + "     VAGUE : "s + std::to_string(vague), 36, sf::Color::White);
	centrer(textScore, centreX, centreY - 20.f);
	dessinerTexte(fenetre, textScore);
	
	auto clignote = chronoClignotement.getElapsedTime().asMilliseconds() % 1000 < 650;
	if(nouveauRecord) {
		if(clignote) {
			auto record = creerTexte("NOUVEAU RECORD !", 40, sf::Color::Yellow);
			centrer(record, centreX, centreY + 40.f);
			dessinerTexte(fenetre, record);
		}
	} else {
		auto record = creerTexte("RECORD : "s + std::to_string(meilleurScore), 26, COULEUR_TITRE);
		centrer(record, centreX, centreY + 40.f);
		dessinerTexte(fenetre, record);
	}
	
	if(chronoGameOver.getElapsedTime().asSeconds() >= DELAI_GAME_OVER) {
		auto aide = creerTexte("ENTREE : REJOUER     ECHAP : MENU", 26, sf::Color{200, 200, 200});
		centrer(aide, centreX, centreY + 130.f);
		dessinerTexte(fenetre, aide);
	}
}

sf::Text Jeu::creerTexte(std::string const& contenu, unsigned taille, sf::Color const& couleur) const {
	auto texte = sf::Text{contenu, police, taille};
	texte.setFillColor(couleur);
	return texte;
}

// Dessine le texte avec une ombre portée pour la lisibilité
void Jeu::dessinerTexte(sf::RenderWindow& fenetre, sf::Text const& texte) const {
	auto ombre = texte;
	ombre.setFillColor(sf::Color{0, 0, 0, texte.getFillColor().a});
	ombre.move(2.f, 2.f);
	fenetre.draw(ombre);
	fenetre.draw(texte);
}

void Jeu::dessinerVoile(sf::RenderWindow& fenetre) const {
	auto voile = sf::RectangleShape{{static_cast<float>(Coordonnees::getLongueurEspace()), static_cast<float>(Coordonnees::getHauteurEspace())}};
	voile.setFillColor(sf::Color{0, 0, 0, 150});
	fenetre.draw(voile);
}

// Initialiser Exception

void Jeu::initialiserExecption(std::exception const& exception) {
	textException = std::make_unique<sf::Text>();
	textException->setFont(police);
	textException->setString(exception.what());
	textException->setFillColor(sf::Color::Red);
}

float Jeu::getFacteurVitesse() const {
	return std::min(2.f, 1.f + 0.1f * (vague - 1));
}

void Jeu::ajouterAsteroides() {
	for (int i = 0; i < vague + 2; ++i) {
		espace.ajouter(std::make_unique<Asteroide>(*this, espace, nullptr, getFacteurVitesse()));
	}
}

void Jeu::gererVagues() {
	if (estEnCours() && !espace.possedeAsteroide()) {
		vague++;
		ajouterAsteroides();
		chronoAnnonce.restart();
	}
}

void Jeu::perdreVie() {
	vies--;
	bonusActif = TypeBonus::AUCUN;
	if (vies <= 0) {
		terminer();
	} else {
		espace.ajouter(std::make_unique<Vaisseau>(*this, espace, COULEUR_VAISSEAU));
	}
}

void Jeu::activerBonus(TypeBonus type) {
	bonusActif = type;
	tempsBonusRestant = DUREE_BONUS;
	message = Bonus::getNom(type);
	chronoMessage.restart();
}

void Jeu::declencherSecousse(float force) {
	forceSecousse = std::max(forceSecousse, force);
}
