#include "Vecteur.h"
#include <cmath>


void Vecteur::operator+=(Vecteur const& autre) {
	x += autre.x;
	y += autre.y;
}

void Vecteur::operator-=(Vecteur const& autre) {
	x -= autre.x;
	y -= autre.y;
}
Vecteur Vecteur::operator+(Vecteur const& autre) const {
	return {x + autre.x, y + autre.y};
}
Vecteur Vecteur::operator-(Vecteur const& autre) const {
	return {x - autre.x, y - autre.y};
}
Vecteur Vecteur::operator*(float coefficient) const {
	return {x * coefficient, y * coefficient};
}
Vecteur Vecteur::creerDepuisAngle(float taille, float angleEnDegre) {
	float angleEnRadian = angleEnDegre * M_PI / 180.f;
    return {taille * std::cos(angleEnRadian), taille * std::sin(angleEnRadian)};
}

