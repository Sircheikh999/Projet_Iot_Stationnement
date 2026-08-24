# Radar de stationnement Arduino

## 1. Description

Ce projet simule un radar de stationnement automobile.
La distance est simulée avec un potentiomètre. Plus le potentiomètre est tourné vers la gauche, plus la distance avec l'obstacle diminue.
Le système mesure la distance, la filtre avec une moyenne des 5 dernières mesures, détermine une zone et agit avec les LED et le buzzer.

## 2. Zones de distance
Seuil n°1 : 160-400 cm (distance), ok (état), verte (led), silence (buzzer);
Seuil n°2 : 45-160 cm (distance), attention (état), jaune (led), bip lent (buzzer);
Seuil n°3 : 0-45 cm (distance), danger (état), rouge (led), bip rapide (buzzer).

## 3. Broches Arduino utilisées 
Led : verte (D8), jaune (D9), rouge (D10);
Bouton (D2); buzzer (D11); potentiomètre (A0).

## 4. Composants

- Arduino Uno
- 3 LED : verte, jaune et rouge
- 3 résistances de 220 ohms
- 1 buzzer
- 1 bouton poussoir
- 1 potentiomètre
- Breadboard
- Fils de connexion

## 5. Fonctionnement

Le potentiomètre simule un capteur de distance.
La valeur brute est convertie en distance de 0 à 400 cm.
Les 5 dernières mesures sont utilisées pour calculer une moyenne afin de stabiliser la mesure.
Le système possède trois états :
- OK
- ATTENTION
- DANGER

Le bouton permet de détecter un événement et utilise un anti-rebond.
Le système utilise également une hystérésis et des temporisations non bloquantes avec "millis()".

## 6. Affichage Serial

Le Serial Monitor affiche :
- la distance brute
- la distance filtrée
- l'état courant
- les événements

## 7. Lancer le projet avec Tinkercad

1. Ouvrir Tinkercad Circuits :
   https://www.tinkercad.com/circuits
2. Créer un nouveau circuit.
3. Ajouter un Arduino Uno, une breadboard et les composants indiqués ci-dessus.
4. Réaliser le câblage en respectant les broches indiquées dans ce README.
5. Ouvrir le mode Code et sélectionner le code texte.
6. Copier le contenu du fichier Arduino ".ino".
7. Coller le code dans Tinkercad.
8. Cliquer sur « Démarrer la simulation ».
9. Ouvrir le Serial Monitor pour observer les distances et les états.
10. Tourner le potentiomètre :
    - droite → distance élevée → OK ;
    - milieu → ATTENTION ;
    - gauche → distance faible → DANGER.

## 8. Tests à effectuer

### Test OK

Positionner le potentiomètre pour obtenir plus de 160 cm.
Résultat :
- LED verte allumée ;
- buzzer silencieux ;
- état OK dans le Serial Monitor.

### Test ATTENTION

Positionner le potentiomètre entre 45 et 160 cm.
Résultat :
- LED jaune allumée ;
- bip lent ;
- état ATTENTION dans le Serial Monitor.

### Test DANGER

Positionner le potentiomètre entre 0 et 45 cm.
Résultat :
- LED rouge allumée ;
- bip rapide ;
- état DANGER dans le Serial Monitor.

### Test du filtrage

Modifier rapidement la position du potentiomètre et observer le Serial Monitor.
La distance brute doit varier plus rapidement que la distance filtrée.

## 9. Robustesse

Le projet utilise :

- un anti-rebond pour le bouton ;
- un filtrage par moyenne de 5 mesures ;
- une hystérésis pour stabiliser les changements d'état ;
- des temporisations non bloquantes avec "millis()".

## 10. Simulation

Lien Tinkercad :
https://www.tinkercad.com/things/3gMKcHsCO1x-miniprojettheme3stationnement?sharecode=Ayd2mBN_DIr7EtTwWt0aJDv6qULmX267ZrJBu1Maxis
Le circuit Tinkercad permet de simuler le montage Arduino et d'observer son comportement avant une réalisation physique.
