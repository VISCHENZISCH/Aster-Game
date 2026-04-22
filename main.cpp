#include <SFML/Graphics.hpp>
#include "Espace.h"
#include "Jeu.h"
#include <exception>

using namespace std;

int main()
{

	
	// Configuration de la fenêtre
    constexpr int LONGUEUR_FENETRE{1200};
    constexpr int HAUTEUR_FENETRE{900};
    
    //: Initialiser l'espace pour les coordonnées
    Coordonnees::initialiserEspace(LONGUEUR_FENETRE, HAUTEUR_FENETRE);
    sf::RenderWindow fenetre(sf::VideoMode{LONGUEUR_FENETRE, HAUTEUR_FENETRE}, "Asteroides");
    auto espace = Espace{};
    auto jeu = Jeu{espace};
	
	
    while(fenetre.isOpen()) {
			try {
				auto evenement = sf::Event{};
				while (fenetre.pollEvent(evenement)){
					if(evenement.type == sf::Event::Closed){
						fenetre.close();
					
					}
					if(evenement.type == sf::Event::KeyPressed && (!jeu.estEnCours())) {
						jeu.demarrer();
					}
						
				}
		
				espace.actualiser();
				espace.gererCollisions();
				
			
				
				fenetre.clear();
				espace.afficher(fenetre);
				jeu.afficher(fenetre);
				fenetre.display();
				
				espace.nettoyer();
			} catch (std::exception const& exception) {
				jeu.initialiserExecption(exception);
			}
    }
   
    return 0;
}
