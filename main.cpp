#include <SFML/Graphics.hpp>
#include "Espace.h"
#include "Jeu.h"
#include "FondEtoile.h"
#include <exception>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{

	
	// Configuration de la fenêtre
    constexpr int LONGUEUR_FENETRE{1200};
    constexpr int HAUTEUR_FENETRE{900};
    
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    //: Initialiser l'espace pour les coordonnées
    Coordonnees::initialiserEspace(LONGUEUR_FENETRE, HAUTEUR_FENETRE);
    sf::RenderWindow fenetre(sf::VideoMode{LONGUEUR_FENETRE, HAUTEUR_FENETRE}, "Asteroides");
    fenetre.setVerticalSyncEnabled(true);
    auto espace = Espace{};
    auto jeu = Jeu{espace};
    auto fond = FondEtoile{150};
	
	sf::View vueOriginale = fenetre.getDefaultView();
	sf::Clock horloge;
	
    while(fenetre.isOpen()) {
			try {
				auto evenement = sf::Event{};
				while (fenetre.pollEvent(evenement)){
					if(evenement.type == sf::Event::Closed){
						fenetre.close();
					}
					jeu.gererEvenement(evenement);
				}
				if(jeu.demandeQuitter()) {
					fenetre.close();
				}
				
				float deltaTime = horloge.restart().asSeconds();
				// En pause, le monde est figé
				if(!jeu.estEnPause()) {
					espace.actualiser();
					espace.gererCollisions();
					jeu.gererVagues();
					fond.actualiser(deltaTime);
				}
				jeu.actualiser(deltaTime);

				sf::View vueActuelle = vueOriginale;
				if (jeu.getForceSecousse() > 0.1f) {
					float dx = ((rand() % 100) / 100.0f - 0.5f) * jeu.getForceSecousse();
					float dy = ((rand() % 100) / 100.0f - 0.5f) * jeu.getForceSecousse();
					vueActuelle.move(dx, dy);
				}
				
				fenetre.clear();
				fond.afficher(fenetre);
				fenetre.setView(vueActuelle);
				espace.afficher(fenetre);
				fenetre.setView(vueOriginale);
				jeu.afficher(fenetre);
				fenetre.display();
				
				espace.nettoyer();
			} catch (std::exception const& exception) {
				jeu.initialiserExecption(exception);
			}
    }
   
    return 0;
}
