#include "Missile.h"
#include <SFML/Audio.hpp>
#include "GestionnaireRessources.h"

Missile::Missile(Coordonnees const& p_position, float rotation) : ElementEspace{"ressources/missile.png"}
{
	
	type = TypeElement::MISSILE;
	position = p_position;
	sprite.setRotation(rotation);
	vitesse = Vecteur::creerDepuisAngle(VITESSE, rotation);
	son.setBuffer(GestionnaireRessources<sf::SoundBuffer>::getRessources("ressources/laser.wav"));
	son.play();
	
}

void Missile::reagirCollision(TypeElement typeAutre){
	if(typeAutre != TypeElement::VAISSEAU) {
		estDetruit = true;
	}
}
