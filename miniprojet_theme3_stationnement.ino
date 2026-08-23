const int LED_VERTE = 8;
const int LED_JAUNE = 9;
const int LED_ROUGE = 10;
const int BUZZER = 11;
const int BOUTON = 2;
const int POT = A0;

// Seuils : OK 101-200 cm, ATTENTION 50-100 cm, DANGER 0-49 cm
const int SEUIL_OK = 100;
const int SEUIL_DANGER = 50;

// Filtrage : moyenne des 5 dernières mesures
float mesures[5];
int indexMesure = 0;

// Hystérésis
const int HYST = 3;

String etat = "OK";

bool ancienBouton = HIGH;
unsigned long dernierAppui = 0;

// Temporisations non bloquantes
unsigned long dernierBip = 0;
unsigned long dernierSerial = 0;

bool buzzer = false;

const unsigned long ANTI_REBOND = 250;
const unsigned long INTERVALLE_SERIAL = 200;
const unsigned long BIP_ATTENTION = 800;
const unsigned long BIP_DANGER = 180;


void setup() {

  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_JAUNE, OUTPUT);
  pinMode(LED_ROUGE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BOUTON, INPUT_PULLUP);

  Serial.begin(9600);

  // Initialisation du filtre
  float distance = map(analogRead(POT), 0, 1023, 200, 0);

  for (int i = 0; i < 5; i++) {
    mesures[i] = distance;
  }

  Serial.println("RADAR DE STATIONNEMENT");
  Serial.println("OK : 101-200 cm");
  Serial.println("ATTENTION : 50-100 cm");
  Serial.println("DANGER : 0-49 cm");
  Serial.println("Droite = loin | Gauche = proche");
  Serial.println();
}


void loop() {

  // Lecture de la distance : droite = 200 cm, gauche = 0 cm
  int valeur = analogRead(POT);

  float brute = map(valeur, 0, 1023, 200, 0);


  // Filtrage : moyenne des 5 dernieres mesures
  mesures[indexMesure] = brute;
  indexMesure = (indexMesure + 1) % 5;

  float somme = 0;

  for (int i = 0; i < 5; i++) {
    somme += mesures[i];
  }

  float distance = somme / 5.0;


  gererBouton();
  changerEtat(distance);
  gererSorties();
  afficherSerial(brute, distance);
}


// 3 états + hystérésis
void changerEtat(float d) {

  String ancienEtat = etat;

  if (etat == "OK") {

    if (d < SEUIL_OK - HYST) {

      if (d < SEUIL_DANGER - HYST) {
        etat = "DANGER";
      }
      else {
        etat = "ATTENTION";
      }
    }
  }

  else if (etat == "ATTENTION") {

    if (d > SEUIL_OK + HYST) {
      etat = "OK";
    }

    else if (d < SEUIL_DANGER - HYST) {
      etat = "DANGER";
    }
  }

  else if (etat == "DANGER") {

    if (d > SEUIL_DANGER + HYST) {

      if (d > SEUIL_OK + HYST) {
        etat = "OK";
      }
      else {
        etat = "ATTENTION";
      }
    }
  }

  if (ancienEtat != etat) {

    Serial.print("EVENEMENT : changement d'etat -> ");
    Serial.println(etat);
  }
}


// LEDs + buzzer
void gererSorties() {

  if (etat == "OK") {

    digitalWrite(LED_VERTE, HIGH);
    digitalWrite(LED_JAUNE, LOW);
    digitalWrite(LED_ROUGE, LOW);

    noTone(BUZZER);
    buzzer = false;
  }

  else if (etat == "ATTENTION") {

    digitalWrite(LED_VERTE, LOW);
    digitalWrite(LED_JAUNE, HIGH);
    digitalWrite(LED_ROUGE, LOW);

    gererBip(BIP_ATTENTION, 1000);
  }

  else {

    digitalWrite(LED_VERTE, LOW);
    digitalWrite(LED_JAUNE, LOW);
    digitalWrite(LED_ROUGE, HIGH);

    gererBip(BIP_DANGER, 2000);
  }
}


// Buzzer non bloquant
void gererBip(unsigned long intervalle, int frequence) {

  if (millis() - dernierBip >= intervalle) {

    dernierBip = millis();

    if (buzzer) {
      noTone(BUZZER);
      buzzer = false;
    }
    else {
      tone(BUZZER, frequence);
      buzzer = true;
    }
  }
}


// Bouton + anti-rebond
void gererBouton() {

  bool bouton = digitalRead(BOUTON);

  if (ancienBouton == HIGH &&
      bouton == LOW &&
      millis() - dernierAppui >= ANTI_REBOND) {

    dernierAppui = millis();

    Serial.print("EVENEMENT : bouton appuye | Etat : ");
    Serial.println(etat);
  }

  ancienBouton = bouton;
}


// Serial : distance brute + filtrée + etat
void afficherSerial(float brute, float filtree) {

  if (millis() - dernierSerial >= INTERVALLE_SERIAL) {

    dernierSerial = millis();

    Serial.print("Brute : ");
    Serial.print(brute, 2);

    Serial.print(" cm | Filtree : ");
    Serial.print(filtree, 2);

    Serial.print(" cm | Etat : ");
    Serial.println(etat);
  }
}