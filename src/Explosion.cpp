#include "Explosion.h"
#include "Vaisseau.h"
#include "GestionnaireRessources.h"

Explosion::Explosion(Coordonnees const& p_position) : ElementEspace("ressources/explosion.png")
{
	position = p_position;
	son.setBuffer(GestionnaireRessources<sf::SoundBuffer>::getRessources("ressources/explosion.wav"));
	son.play();
}

// Mettre à jour l'explosion 
 void Explosion::mettreAJour(float temps) {
 	age += temps;
 	if (age < DUREE_VIE) {
		sprite.setScale(age/DUREE_VIE, age/DUREE_VIE);
		
 	} else {
 		sprite.setScale(0,0);
 		if(son.getStatus() == sf::SoundSource::Stopped){
			estDetruit = true;
 		}
 	}
 	
 }
 


 // Réagir à une collision
 
 void Explosion::reagirCollision(TypeElement typeAutre) {}
 