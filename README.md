# Asteroides

Un jeu de tir spatial classique développé en **C++** en utilisant la bibliothèque **SFML**.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![SFML](https://img.shields.io/badge/SFML-2.5.1-green.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

##Aperçu

**Asteroides** est un projet de jeu d'arcade où vous contrôlez un vaisseau spatial naviguant à travers un champ d'astéroïdes dangereux. Votre objectif est de survivre le plus longtemps possible tout en détruisant les astéroïdes pour marquer des points.

## Fonctionnalités

- **Moteur de jeu personnalisé** : Gestion de l'espace, des mouvements et des collisions.
- **Physique simplifiée** : Inertie du vaisseau et rotation.
- **Système de score** : Suivi du meilleur score localement dans `meilleur_score.txt`.
- **Gestion des assets** : Chargement dynamique des textures et sons (via SFML).
- **Gestion des exceptions** : Système robuste pour capturer et afficher les erreurs de runtime.

## Prérequis

Pour compiler et exécuter ce projet, vous aurez besoin de :

- Un compilateur C++ (compatible C++17 ou supérieur).
- [SFML (Simple and Fast Multimedia Library)](https://www.sfml-dev.org/) installé sur votre système.
- [Code::Blocks](http://www.codeblocks.org/) (optionnel, le projet contient un fichier `.cbp`).

## Compilation et Installation

### Utilisation de Code::Blocks
1. Ouvrez le fichier `Asteroides.cbp` dans Code::Blocks.
2. Assurez-vous que les chemins vers les bibliothèques SFML sont correctement configurés dans *Build Options*.
3. Compilez et lancez le projet (F9).

### Ligne de commande (Linux)
Si vous avez installé SFML via les dépôts (ex: `sudo apt install libsfml-dev`), vous pouvez compiler manuellement :

```bash
g++ -c main.cpp src/*.cpp -Iinclude
g++ *.o -o Asteroides -lsfml-graphics -lsfml-window -lsfml-system
./Asteroides
```

---

## Structure du Projet

- `src/` : Fichiers sources (`.cpp`).
- `include/` : Fichiers d'en-tête (`.h`).
- `ressources/` : Images, sons et polices d'écriture.
- `bin/` : Exécutables compilés.
- `obj/` : Fichiers objets intermédiaires.

## Auteur

Développé par **TOVIGNAN Félix**.
