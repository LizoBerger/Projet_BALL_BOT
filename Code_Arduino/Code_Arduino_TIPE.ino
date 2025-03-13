#define FORWARD 'F'
#define BACKWARD 'B' 
#define LEFT 'L' 
#define RIGHT 'R' 
#define PAUSE 'P' 
#define TIR 'O' 
#define STOP 'S' 

#include <SoftwareSerial.h> // Bibliothèque pour la communication série logicielle

SoftwareSerial bluetoothSerial(8, 9); // RX, TX 
int moteur_gauche_forward = 13; 
int moteur_gauche_backward = 12; 
int moteur_droit_forward = 11; 
int moteur_droit_backward = 10; 
int valeur_phototransistor = A0; 
int luminosite = 0; 
int laser = 7;
int seuil = 100;
unsigned long time = 0; 



void setup(){ 
  Serial.begin(9600);
  bluetoothSerial.begin(9600); 

  pinMode(moteur_gauche_forward, OUTPUT);
  digitalWrite(moteur_gauche_forward, LOW);
  pinMode(moteur_gauche_backward, OUTPUT);
  digitalWrite(moteur_gauche_backward, LOW);

  pinMode(moteur_droit_forward, OUTPUT);
  digitalWrite(moteur_droit_forward, LOW);
  pinMode(moteur_droit_backward, OUTPUT);
  digitalWrite(moteur_droit_backward, LOW);

  pinMode(valeur_phototransistor,INPUT);

  pinMode(laser, OUTPUT); 
  digitalWrite(laser, LOW);

} 



void loop() {

  int valeur_lue = analogRead(valeur_phototransistor); 

  if (valeur_lue<=seuil){ 
    luminosite =1;
  }
  else{ 
    luminosite = 0; 
  } 

  Serial.println(luminosite);
  delay(10);

  if (bluetoothSerial.available() > 0) { 
    char command = bluetoothSerial.read();
    executeCommand(command);
  }

  if ((millis()-time > 3000) && (digitalRead(laser) == HIGH)){
    digitalWrite(laser, LOW);
  }

  if (luminosite==1) {
    digitalWrite(moteur_gauche_forward, LOW);
    digitalWrite(moteur_droit_forward, LOW);
    digitalWrite(moteur_gauche_backward, LOW);
    digitalWrite(moteur_droit_backward, LOW);
    digitalWrite(laser, LOW);
    delay(3000);
  }
}

void executeCommand(char command){

  switch(command){

    case FORWARD:

    digitalWrite(moteur_gauche_forward, HIGH);
    digitalWrite(moteur_droit_forward, HIGH);
    digitalWrite(moteur_gauche_backward, LOW);
    digitalWrite(moteur_droit_backward, LOW);
    break; 

    case STOP:

    digitalWrite(moteur_gauche_forward, LOW);
    digitalWrite(moteur_droit_forward, LOW);
    digitalWrite(moteur_gauche_backward, LOW);
    digitalWrite(moteur_droit_backward, LOW);
    break;

    case BACKWARD:

    digitalWrite(moteur_gauche_forward, LOW);
    digitalWrite(moteur_droit_forward, LOW);
    digitalWrite(moteur_gauche_backward, HIGH);
    digitalWrite(moteur_droit_backward, HIGH);
    break;


    case LEFT:

    digitalWrite(moteur_gauche_forward, LOW);
    digitalWrite(moteur_droit_forward, HIGH);
    digitalWrite(moteur_gauche_backward, HIGH);
    digitalWrite(moteur_droit_backward, LOW);
    break;

    case RIGHT:

    digitalWrite(moteur_gauche_forward, HIGH);
    digitalWrite(moteur_droit_forward, LOW);
    digitalWrite(moteur_gauche_backward, LOW);
    digitalWrite(moteur_droit_backward, HIGH);
    break;

    case TIR:

    Serial.print(time);

    if (millis()-time > 10000){
      digitalWrite(laser, HIGH);
      time = millis();
    }
    break;

    case PAUSE:

    digitalWrite(moteur_gauche_forward, LOW);
    digitalWrite(moteur_droit_forward, LOW);
    digitalWrite(moteur_gauche_backward, LOW);
    digitalWrite(moteur_droit_backward, LOW);
    digitalWrite(laser, LOW);
    break;

    default: 

    break; 
    } 
}
