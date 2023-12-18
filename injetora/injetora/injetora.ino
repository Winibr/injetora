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

// Pinagem do Driver A4988:
const int DIR = 2, STEP = 3;

// Variáveis para comunicação com o motor de passo:
int pastaTotal, pastaUtilizada, pastaAcumulada;

// Variáveis para comunicação com a interface do usuário:
float mlPastaTotal, mlPastaUtilizada, mlPastaAcumulada;

int menu(String mensagem) {

  float saida;
  int valor = 0;
  while (true) {
    int botao = analogRead(A0);
    saida = valor / 10.0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(mensagem);
    delay(100);

    // botao Direita:
    if (botao <= 60) {
      valor++;
    }

    // botao Cima:
    else if (botao <= 200) {
      valor++;
    }

    // botao Baixo:
    else if (botao <= 400) {
      valor--;
    }

    // botao Esquerda:
    else if (botao <= 600) {
      valor--;
    }

    // botao SELECIONAR:
    else if (botao <= 800) {
      return valor;
    }

    lcd.setCursor(0, 1);
    lcd.print(saida);
    delay(100);
  }
}

int config() {
  pastaTotal = menu("solda total (ml):");
}

void dispersar(int valor) {
  for (int i = 0; i < valor; i++) {
    digitalWrite(DIR, HIGH);
    digitalWrite(STEP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP, LOW);
    delayMicroseconds(500);
  }
  pastaTotal = pastaTotal - valor;
}

void setup() {
  // Configurando o LCD keypad shield:
  lcd.begin(16, 2);
  // Configurando o Driver A4988:
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  // Obtendo a quantidade de solda do usuário
  config();
}

void loop() {
  int solicitacaoUsuario = menu("Qtd para usar (ml):");
  if (solicitacaoUsuario <= pastaTotal) {  // 5  //15
    dispersar(solicitacaoUsuario);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    [ERRO!]  ");
    lcd.setCursor(0, 1);
    lcd.print(" Limite Atingido!");
    delay(10000000);
    exit(0);
  }
}
