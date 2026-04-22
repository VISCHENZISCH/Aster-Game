#include "Espace.h"
#include <iostream>

Espace::Espace()
{
	//ctor
}
// ajouter des éléments de l'espace
 void Espace::ajouter(std::unique_ptr<ElementEspace> element) {
 	aAjouter.push_back(std::move(element));
 }
 
 // Actualiser Element de l'espace 
 void Espace::actualiser() {
	auto tempsBoucle = chrono.restart().asSeconds();
	for (auto i{0u}; i < elements.size(); ++i) {
		elements[i]->actualiser(tempsBoucle);
	}
 }
 
 // Gérer Collisions
 void Espace::gererCollisions() {
 	for (auto i{0u}; i < elements.size(); ++i) {
			for (auto j{0u}; j <  elements.size(); ++j) {
				if(i != j) {
					elements[i]->testerCollision(*elements[j]);
			}
		}
	}
 }
 
 // Affciher 
 
 void Espace::afficher(sf::RenderWindow& fenetre) const {
 	for (auto& element : elements) {
			element->afficher(fenetre);
		}
 }
 
 void Espace::nettoyer() {
 	auto finTableau = std::remove_if(std::begin(elements), std::end(elements), ElementEspace::elementEstDetruit );
	elements.erase(finTableau, std::end(elements));
	for(auto& element : aAjouter) {
			elements.push_back(std::move(element));
	}
	aAjouter.clear();
 }
 
 // Vider l'espace 
 
void Espace::vider() {
	elements.clear();
	aAjouter.clear();
}