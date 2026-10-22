#ifndef GESTIONNAIRERESSOURCES_H
#define GESTIONNAIRERESSOURCES_H

#include <iostream>
#include "ExceptionRessourceIntrouvable.h"
#include <string_view>
#include <string>
#include <unordered_map>


template <typename T>
class GestionnaireRessources
{
	public:
		GestionnaireRessources() = delete;
		static T const& getRessources(std::string_view const& chemin) {
				auto cle = std::string{chemin};
				auto resultat = ressources.find(cle);
				if(resultat == std::end(ressources)) {
					if(!ressources[cle].loadFromFile(cle)) {
							ressources.erase(cle);
							throw ExceptionRessourceIntrouvable{chemin};
					}
					return ressources[cle];
				}
				return resultat->second;
		}

	protected:

	private:
		static std::unordered_map<std::string, T> ressources;
};

#endif // GESTIONNAIRERESSOURCES_H
