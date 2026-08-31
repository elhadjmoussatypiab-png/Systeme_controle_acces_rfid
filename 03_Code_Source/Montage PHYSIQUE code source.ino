
#include <LiquidCrystal.h>
#include<Servo.h>
#include <SPI.h>
#include <MFRC522.h>

Servo porte;
const int buzzer = A0;

LiquidCrystal lcd(8,7,6,5,4,3);

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);


int monChiffreAutorise[]={243,4};
int nombrecarte =2;

int LED_BLANCHE = A2;
int LED_ROUGE = A1;

void setup() {
Serial.begin(9600); // Pour voir le chiffre sur le PC
SPI.begin();
rfid.PCD_Init(); // Démarre le lecteur RFID

Serial.println("Lecteur prêt ! Approchez une carte...");

  lcd.begin(16,2);
  affichebonjour();
  
  pinMode(buzzer, OUTPUT);
  porte.attach(2);
 

pinMode(A2, OUTPUT);
pinMode(A1, OUTPUT);

digitalWrite(A2, LOW);
digitalWrite(A1, LOW);
}

void loop() {
  int resultat = codetape();
  if (resultat == 1){
    affichecorrect();
    autorisation ();
    
  
    delay(2000);
    noTone(buzzer);
    lcd.clear();
    affichebonjour();
    
  }
  else if (resultat == 2) {
    afficheincorrect();
    bip_refus ();
    
    delay(2000);
    lcd.clear();
    affichebonjour();
  }
  
    
}




void affichebonjour(){
  lcd.setCursor(3,0);
  lcd.print("Presenter");
  lcd.setCursor(2,1);
  lcd.print("Votre Carte");
}

void affichecorrect(){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("CARTE CORRECTE");
            lcd.setCursor(1,1);
            lcd.print("PORTE OUVERTE");

}

void afficheincorrect(){
              lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("CARTE INCORRECTE");
            lcd.setCursor(0,1);
            lcd.print("ACCES REFUSE");

}

void allumefaux(){
         digitalWrite(LED_BLANCHE, LOW);
         digitalWrite(LED_ROUGE, HIGH);

}

void allumevraie(){
         digitalWrite(LED_BLANCHE,HIGH);
         digitalWrite(LED_ROUGE, LOW);
         

}


void autorisation (){ 
  tone(buzzer, 600);
  allumevraie();
  porte.write(180);
  
  delay(2000);
  
  noTone(buzzer);
  porte.write(90);
  digitalWrite(LED_BLANCHE, LOW);
  digitalWrite(LED_ROUGE, LOW);
  

}

void bip_refus (){
  allumefaux();
  for (int i=0; i<3; i++){
   tone(buzzer, 600);
    delay(200);
    noTone(buzzer);
    delay(150);
    }
  delay(1000);
  digitalWrite(LED_BLANCHE, LOW);
  digitalWrite(LED_ROUGE, LOW);
}

int codetape(){
// 1. Attente d'une carte devant le lecteur
if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
return 0;
}

// 2. Récupération du 1er chiffre de la carte détectée
int chiffreDuBadge = rfid.uid.uidByte[0];

// Affichage du chiffre dans le moniteur série
Serial.print("Chiffre de cette carte : ");
Serial.println(chiffreDuBadge);

  for (int i=0 ; i < nombrecarte; i++){
     if (chiffreDuBadge == monChiffreAutorise[i]) {
       rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        return 1 ;
       } 
  }
    rfid.PICC_HaltA();
     rfid.PCD_StopCrypto1();
  
 return 2;
}







