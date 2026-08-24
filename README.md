# Projet Arduino — Radar de distance

## COMMENT LANCER

### 1. Présentation du projet

Ce projet réalise une petite simulation de radar de stationnement avec un **Arduino Uno**. Le potentiomètre représente une distance simulée : lorsqu’on le tourne vers la position proche, le système devient plus urgent. Le programme lit la mesure, la stabilise, choisit une zone et agit avec des LED et un buzzer.

La consigne autorise un potentiomètre comme distance simulée à la place d’un capteur ultrason. Les distances affichées en centimètres sont donc calculées à partir de la position du potentiomètre ; il ne s’agit pas d’une mesure physique.

### 2. Fonctionnement attendu

| Zone | Distance filtrée | LED | Buzzer |
|---|---|---|---|
| **OK** | À partir d’environ 160 cm | Verte | Silence |
| **ATTENTION** | Environ 45 à moins de 160 cm | Jaune | Bips lents |
| **DANGER** | Moins d’environ 45 cm | Rouge | Bips rapides |
| **ERREUR_CAPTEUR** | Valeur impossible ou hors plage | Aucune LED de zone | Diagnostic sonore et série |

Le programme calcule une **moyenne glissante des cinq dernières valeurs**. Une **hystérésis** évite les changements répétés près d’un seuil. Le bouton est **antirebond** et les bips sont temporisés avec `millis()`, sans bloquer le programme avec des `delay()` répétés.

### 3. Composants

| Composant | Quantité | Rôle |
|---|---:|---|
| Arduino Uno | 1 | Contrôleur |
| Petite breadboard | 1 | Montage des composants |
| Potentiomètre | 1 | Distance simulée |
| Bouton-poussoir carré à quatre pattes | 1 | Changement de mode |
| LED verte, jaune et rouge | 1 de chaque | Indication des zones |
| Résistance 220 Ω | 3 | Une par LED |
| Buzzer piézoélectrique | 1 | Alerte sonore |
| Fils de connexion | Plusieurs | Alimentation et signaux |

### 4. Broches Arduino

| Broche | Connexion |
|---|---|
| **A0** | Curseur central du potentiomètre |
| **5V** | Borne extérieure du potentiomètre et rail rouge `+` |
| **GND** | Borne extérieure opposée et rail bleu `−` |
| **D2** | Bouton-poussoir avec `INPUT_PULLUP` |
| **D8** | LED verte via une résistance de 220 Ω |
| **D9** | LED jaune via une résistance de 220 Ω |
| **D10** | LED rouge via une résistance de 220 Ω |
| **D11** | Borne positive du buzzer |
| **GND** | Cathodes des LED, buzzer et retour du bouton `−` |

#### Règles de câblage importantes

Chaque LED se branche ainsi :

```text
Broche Arduino → résistance 220 Ω → patte longue de la LED
Patte courte de la LED → rail bleu GND
```

Le bouton doit être placé à cheval sur la rainure centrale. Un côté va vers **D2** et l’autre vers le rail bleu **GND**. Aucun fil du bouton ne doit aller directement au rail rouge `+`.

Le potentiomètre se branche avec une borne extérieure au **5V**, le curseur central à **A0** et l’autre borne extérieure au **GND**.

Les rails rouge et bleu restent séparés.

### 5. Lancer le projet dans Tinkercad

1. Créer le circuit.
2. Relier l’alimentation : **5V** vers le rail rouge `+` et **GND** vers le rail bleu `−`. Ne jamais relier directement les deux rails.
3. Relier le potentiomètre : borne extérieure vers **5V**, curseur vers **A0** et autre borne extérieure vers **GND**.
4. Placer le bouton à cheval sur la rainure centrale, puis relier un côté à **D2** et l’autre au rail **GND**.
5. Relier les LED : verte **D8**, jaune **D9**, rouge **D10**, avec une résistance de **220 Ω** pour chacune. Les pattes courtes vont au **GND**.
6. Relier la borne `+` du buzzer à **D11** et sa borne `−` au rail **GND**.
7. Dans **Code**, choisir **Texte**, supprimer l’ancien contenu, coller tout le fichier `.ino` et démarrer la simulation.
8. Ouvrir le **moniteur série**.

Tourner le potentiomètre vers la droite simule une distance éloignée ; le tourner vers la gauche simule une distance proche.

Appuyer brièvement sur le bouton permet de parcourir les modes **AUTO**, **CALIBRATION** et **TEST**.

### 6. Exemple de sortie série

```text
distance_brute_cm=92.0; distance_filtree_cm=98.4; zone=ATTENTION; mode=AUTO; buzzer=ON
```

Cette ligne est normale : `mode=AUTO` signifie que le système fonctionne automatiquement et `buzzer=ON` signifie qu’un bip est actif à cet instant.

En zone **ATTENTION**, le buzzer alterne ensuite avec `buzzer=OFF` pendant la pause entre les bips.

### 7. Tests obligatoires

| Test | Manipulation | Résultat attendu |
|---|---|---|
| **OK** | Potentiomètre vers la droite, environ 250–400 cm | LED verte et silence |
| **ATTENTION** | Potentiomètre au milieu, environ 80–120 cm | LED jaune et bips lents |
| **DANGER** | Potentiomètre vers la gauche, environ 10–35 cm | LED rouge et bips rapides |
| **Filtrage** | Faire varier rapidement le potentiomètre | La valeur brute bouge davantage que la valeur filtrée |
| **Hystérésis** | Rester autour de 50–55 cm | La zone ne change pas à chaque petite oscillation |
| **Antirebond** | Appuyer brièvement une fois | Un seul changement de mode |

### 8. Extension facultative : Arduino → Serial → PC → SQLite

Cette extension n’est pas nécessaire pour valider la tâche principale. Elle permet d’enregistrer les mesures et les événements dans une base **SQLite**.

#### Sans Arduino réel

Ouvrir un terminal dans le dossier contenant `collector.py` et exécuter :

```bash
python collector.py --init-db --db radar.db
python collector.py --db radar.db --input exemples_serial.txt
```

Si la commande d’import est exécutée deux fois, les mêmes mesures seront enregistrées deux fois. Ce n’est pas une erreur : la requête comptera alors deux mesures par zone.

#### Avec un Arduino physique sous Windows

Installer `pyserial` :

```bash
python -m pip install pyserial
```

Puis lancer, par exemple :

```bash
python collector.py --port COM3 --baud 9600 --db radar.db
```

> **Important :** le moniteur série Arduino et `collector.py` ne doivent pas ouvrir le même port en même temps.

Tinkercad ne fournit généralement pas un vrai port COM au PC ; dans ce cas, utiliser `exemples_serial.txt`.

#### Requête SQL de démonstration

```sql
SELECT zone, COUNT(*) AS nombre_mesures
FROM mesures
GROUP BY zone
ORDER BY nombre_mesures DESC;
```

### 9. Références

**Lien Tinkercad :**

https://www.tinkercad.com/things/3gMKcHsCO1x-miniprojettheme3stationnement?sharecode=Ayd2mBN_DIr7EtTwWt0aJDv6qULmX267ZrJBu1Maxis
