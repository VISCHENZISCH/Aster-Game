#ifndef ELEMENTESPACE_H
#define ELEMENTESPACE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Vecteur.h"
#include "Coordonnees.h"

enum class TypeElement {VAISSEAU, ASTEROIDE, MISSILE, AUTRE};
class ElementEspace
{
	public:
		virtual ~ElementEspace() = default;
		explicit ElementEspace(std::string_view const& cheminImage);
		ElementEspace(ElementEspace const& autre) = delete;
		void operator=(ElementEspace const& autre) = delete;
		virtual void afficher(sf::RenderWindow& fenetre) const;
		void testerCollision(ElementEspace& autre);
		void actualiser(float temps);
		static inline bool elementEstDetruit(std::unique_ptr<ElementEspace>& element) { return element->estDetruit;};
		// Méthodes virtual Pure 
		virtual void reagirCollision(TypeElement typeAutre) = 0;
		float getRayon() const;
		
	protected:
		
		virtual void mettreAJour(float temps);
		TypeElement type{TypeElement::AUTRE};
		//sf::Texture texture{};
		sf::Sprite sprite{};
		Coordonnees position{};
		Vecteur vitesse{0.f, 0.f};
		float vitesseAngulaire{};
		bool estDetruit{false};
};

#endif // ELEMENTESPACE_H
