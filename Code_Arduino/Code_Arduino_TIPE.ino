// Définition des commandes pour contrôler le robot
#define FORWARD 'F'    // Avancer
#define BACKWARD 'B'   // Reculer
#define LEFT 'L'       // Tourner à gauche
#define RIGHT 'R'      // Tourner à droite
#define PAUSE 'P'      // Pause (arrêt des moteurs et du laser)
#define TIR 'O'        // Activer le tir (laser)
#define STOP 'S'       // Stopper le robot

#include <SoftwareSerial.h> // Bibliothèque pour la communication série logicielle avec le module Bluetooth

// Initialisation de la communication Bluetooth avec des broches RX (8) et TX (9)
SoftwareSerial bluetoothSerial(8, 9); 

// Définition des broches des moteurs pour les deux moteurs du robot (avant/arrière)
int moteur_gauche_forward = 13;   // Broche moteur gauche (avant)
int moteur_gauche_backward = 12;  // Broche moteur gauche (arrière)
int moteur_droit_forward = 11;    // Broche moteur droit (avant)
int moteur_droit_backward = 10;   // Broche moteur droit (arrière)

// Broche du phototransistor (capteur de lumière) et de la sortie laser
int valeur_phototransistor = A0;  // Broche de lecture du phototransistor
int luminosite = 0;               // Variable pour stocker l'état de la luminosité
int laser = 7;                    // Broche pour contrôler le laser
int seuil = 100;                  // Seuil de luminosité pour l'immobilisation
unsigned long time = 0;           // Variable pour gérer le délai pour le laser

void setup() { 
  // Initialisation des communications série (USB et Bluetooth)
  Serial.begin(9600);             // Communication série avec l'ordinateur (pour le debug)
  bluetoothSerial.begin(9600);    // Communication série avec le module Bluetooth (vitesse de transmission)

  // Configuration des broches des moteurs et laser comme sorties
  pinMode(moteur_gauche_forward, OUTPUT);
  digitalWrite(moteur_gauche_forward, LOW);  // Moteur gauche en position "arrêt"
  
  pinMode(moteur_gauche_backward, OUTPUT);
  digitalWrite(moteur_gauche_backward, LOW); // Moteur gauche en position "arrêt"
  
  pinMode(moteur_droit_forward, OUTPUT);
  digitalWrite(moteur_droit_forward, LOW);  // Moteur droit en position "arrêt"
  
  pinMode(moteur_droit_backward, OUTPUT);
  digitalWrite(moteur_droit_backward, LOW); // Moteur droit en position "arrêt"

  pinMode(valeur_phototransistor, INPUT); // Le phototransistor est en entrée

  pinMode(laser, OUTPUT); 
  digitalWrite(laser, LOW); // Laser éteint au départ
}

void loop() {
  // Lecture de la valeur du phototransistor (capteur de lumière)
  int valeur_lue = analogRead(valeur_phototransistor);

  // Comparaison de la valeur lue avec le seuil pour déterminer l'état de luminosité
  if (valeur_lue <= seuil) {
    luminosite = 1; // Faible luminosité (capteur détecte un faible éclairage)
  } else {
    luminosite = 0; // Haute luminosité (capteur détecte un éclairage suffisant)
  }

  Serial.println(luminosite);  // Affichage de l'état de la luminosité sur le moniteur série
  delay(10);                   // Petit délai pour éviter des lectures trop rapides

  // Vérification de la réception d'une commande Bluetooth
  if (bluetoothSerial.available() > 0) { 
    char command = bluetoothSerial.read(); // Lecture de la commande reçue via Bluetooth
    executeCommand(command);  // Exécution de la commande
  }

  // Gestion de la temporisation du laser (éteindre après 3 secondes)
  if ((millis() - time > 3000) && (digitalRead(laser) == HIGH)) {
    digitalWrite(laser, LOW); // Eteindre le laser après 3 secondes
  }

  // Si faible luminosité (luminosite == 1), on arrête tous les moteurs et le laser pendant 3 secondes
  if (luminosite == 1) {
    digitalWrite(moteur_gauche_forward, LOW);
    digitalWrite(moteur_droit_forward, LOW);
    digitalWrite(moteur_gauche_backward, LOW);
    digitalWrite(moteur_droit_backward, LOW);
    digitalWrite(laser, LOW);
    delay(3000); // Pause de 3 secondes avant de reprendre l'exécution
  }
}

// Fonction pour exécuter la commande reçue
void executeCommand(char command) {
  switch(command) {
    case FORWARD:
      // Faire avancer les moteurs
      digitalWrite(moteur_gauche_forward, HIGH);
      digitalWrite(moteur_droit_forward, HIGH);
      digitalWrite(moteur_gauche_backward, LOW);
      digitalWrite(moteur_droit_backward, LOW);
      break; 

    case STOP:
      // Arrêter tous les moteurs
      digitalWrite(moteur_gauche_forward, LOW);
      digitalWrite(moteur_droit_forward, LOW);
      digitalWrite(moteur_gauche_backward, LOW);
      digitalWrite(moteur_droit_backward, LOW);
      break;

    case BACKWARD:
      // Faire reculer les moteurs
      digitalWrite(moteur_gauche_forward, LOW);
      digitalWrite(moteur_droit_forward, LOW);
      digitalWrite(moteur_gauche_backward, HIGH);
      digitalWrite(moteur_droit_backward, HIGH);
      break;

    case LEFT:
      // Tourner à gauche
      digitalWrite(moteur_gauche_forward, LOW);
      digitalWrite(moteur_droit_forward, HIGH);
      digitalWrite(moteur_gauche_backward, HIGH);
      digitalWrite(moteur_droit_backward, LOW);
      break;

    case RIGHT:
      // Tourner à droite
      digitalWrite(moteur_gauche_forward, HIGH);
      digitalWrite(moteur_droit_forward, LOW);
      digitalWrite(moteur_gauche_backward, LOW);
      digitalWrite(moteur_droit_backward, HIGH);
      break;

    case TIR:
      // Activation du laser avec restriction de 10secs d'intervalle entre 2 tirs
      Serial.print(time);
      if (millis() - time > 10000) {
        digitalWrite(laser, HIGH); // Allumer le laser
        time = millis(); // Réinitialiser le temps
      }
      break;

    case PAUSE:
      // Arrêter tous les moteurs et éteindre le laser
      digitalWrite(moteur_gauche_forward, LOW);
      digitalWrite(moteur_droit_forward, LOW);
      digitalWrite(moteur_gauche_backward, LOW);
      digitalWrite(moteur_droit_backward, LOW);
      digitalWrite(laser, LOW);
      break;

    default: 
      // Commande non reconnue, aucune action effectuée
      break; 
  } 
}
