#include "ExceptionRessourceIntrouvable.h"


using namespace std::string_literals;

ExceptionRessourceIntrouvable::ExceptionRessourceIntrouvable(std::string_view const& chemin) : messageErreur("Erreur, resource introuvable : "s + chemin.data())
{
	//ctor
}

const char* ExceptionRessourceIntrouvable::what() const noexcept  {
	return messageErreur.c_str();

}
