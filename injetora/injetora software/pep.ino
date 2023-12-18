/*
 * Injetor de Solda em Pasta
 * Feito por Winícius Benevides Rodrigues
 *
 * Shield LCD Keypad:
 *
 * RS               = 8
 * Enable           = 9
 * D7               = 7
 * D6               = 6
 * D5               = 5
 * D4               = 4
 * botao            = A0
 *
 * Valores analógicos do LCD KEYPAD SHIELD (A0):
 *
 * DIREITA         = 0 - 60
 * CIMA            = 60 - 200
 * BAIXO           = 200 - 400
 * ESQUERDA        = 400 - 600
 * SELECIONAR      = 600 - 800
 *
 * Motor de Passo A4988:
 * DIR = 2
 * PASSO = 3
 *
 * 1 Revolução = 200 passos
 * 200 passos = 1,25 mm para frente
 * 1,25 mm para frente = 0,20 ml na seringa
 * 100 passos = 0,10 ml
 *
 *
 * 160 passos = 1 mm
 * 1 Passo = 1,8 graus
 */

#include <LiquidCrystal.h>
// Pinagem do LCD Keypad Shield:
const int RS = 8, EN = 9, D4 = 4, D5 = 5, D6 = 6, D7 = 7;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

int button = analogRead(A0);

int totalpaste = 0;

// Pinagem do Driver A4988:
const int DIR = 2, STEP = 3;

int cursor(String msg){

  int value = 0;

  while(true){
    lcd.clear();

    int button = analogRead(A0);
    
    if(button > 0){
      //add
      if(button <= 200){
        value++;
      }
      
      //subtract
      else if(button <= 600){
        value--;
      }
      //select
      else if(button <= 800){
        return value;
      }
      lcd.setCursor(0, 0);
      lcd.print(msg);
      Serial.println(value/10.0);
      lcd.setCursor(0, 1);
      lcd.print(value/10.0);
      lcd.print("ml");
      delay(100); 
    }
  }
}

void setup(){
  //setting the lcd up:
  lcd.begin(16, 2);

  //serial print:
  Serial.begin(9600);

  //setting the stepper motors:
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);

  //setting up the amount of solder used
  totalpaste = cursor("Total value:");
}

void dispense(int value){
  int step = value * 100;
  digitalWrite(DIR, HIGH);

  for (int i = 0; i < step; i++){
    digitalWrite(STEP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP, LOW);
    delayMicroseconds(500);
  }
}

void loop(){
  int userpaste = cursor("Used value:");
  int remainingpaste = totalpaste - userpaste;
  if (remainingpaste > 0){
    Serial.print("valid amount ");
    Serial.println(remainingpaste/10.0);
    dispense(userpaste);
    totalpaste = totalpaste - userpaste;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Remaining: ");
    lcd.print(totalpaste/10);
    lcd.print("ml");
    delay(3000);
  }

  else{
    Serial.print("ERROR  ");
    Serial.println(remainingpaste/10);
  }
  delay(100);
}
