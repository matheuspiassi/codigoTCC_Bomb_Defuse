#include <Keypad.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

const byte LINHAS = 4; 
const byte COLUNAS = 4; 

const char TECLAS_MATRIZ[LINHAS][COLUNAS] = { 
  {'*', '7', '4', '1'},
  {'0', '8', '5', '2'},   
  {'#', '9', '6', '3'},
  {'D', 'c', 'B', 'A'}
};

const byte PINOS_LINHAS[LINHAS] = {9, 8, 7, 6}; 
const byte PINOS_COLUNAS[COLUNAS] = {5, 4, 3, 2}; 

Keypad teclado_personalizado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS); 
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {
  lcd.init();
  pinMode(13, OUTPUT);
}

void loop() {
      digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
        delay(100);

  char leitura_teclas = teclado_personalizado.getKey(); 

  lcd.setBacklight(HIGH);
  if (leitura_teclas) { 
    lcd.print(leitura_teclas); 
  }
  
}