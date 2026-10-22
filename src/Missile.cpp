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
	son.setVolume(40.f);
	son.play();
	
}

// Le missile disparaît au bout de sa durée de vie
void Missile::mettreAJour(float temps) {
	age += temps;
	if(age > DUREE_VIE) {
		estDetruit = true;
	}
}

void Missile::reagirCollision(TypeElement typeAutre){
	if(typeAutre == TypeElement::ASTEROIDE) {
		estDetruit = true;
	}
}
