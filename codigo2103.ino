#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TM1637Display.h>
const int numLinhas = 4;
const int numColunas = 4;
char keymap[numLinhas][numColunas] = {
  { 'D', '#', '0', '*' },
  { 'C', '9', '8', '7' },
  { 'B', '6', '5', '4' },
  { 'A', '3', '2', '1' }
};
byte rowPins[numLinhas] = { 43, 45, 47, 49 };
byte colPins[numColunas] = { 35, 37, 39, 41 };

int pinoLedVermelho = 8;
int pinoLedVerde = 9;

int estadoBomba = 0;
int c, i;
bool flag = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
const char NO_KEY = '\0';

const char* senhaCorreta = "132711";
char entradaSenha[7];

#define CLK_PIN 11
#define DIO_PIN 12

TM1637Display display(CLK_PIN, DIO_PIN);

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(pinoLedVermelho, OUTPUT);
  pinMode(pinoLedVerde, OUTPUT);
  i = 0;
  c = 10000;
  display.setBrightness(7);
}

unsigned long tempoAnterior = 0;
const unsigned long intervalo = 1000;
void displayNumber(int number) {
  display.showNumberDec(number, true);
}

void atualizaDisplay() {
//Codigo para cronometrar
  /* if (flag){
    i++;
    display.showNumberDec(c+i);
   
  
    if (i==59)
    {
      i=-1;
      c=c+100;
    }
    Serial.println(c+i);
    delay(1000);
  }*/
  unsigned long tempoAtual = millis();

  tempoAtual - tempoAnterior >= intervalo;

      tempoAnterior = tempoAtual;

  unsigned long tempoSegundos = tempoAtual / 1000;

  unsigned int minutos = tempoSegundos / 60;

  unsigned int segundos = tempoSegundos % 60;

  int displayTime = (minutos * 100) + segundos;

  display.showNumberDecEx(displayTime, 0b01000000, true);
}
void dadosTeclado() {
  //Codigo para ler dados do teclado
  int t = 0;
  while (t < 6) {
    char key = getKey();
    if (key != NO_KEY) {
      entradaSenha[t++] = key;
      lcd.setCursor(t - 1, 1);
      lcd.print(key);
    }
    atualizaDisplay();
    delay(200);
  }
}

void verificacaoSenha() {
  //Codigo para conferir senha
  if (strcmp(entradaSenha, senhaCorreta) == 0) {
    atualizaDisplay();
    exibirMensagemLCD("Senha Correta!");
    exibirMensagemLCD3("Bomba  Desarmada");
    digitalWrite(pinoLedVerde, LOW);
    digitalWrite(pinoLedVermelho, HIGH);
    estadoBomba = 1;

  } else {
    exibirMensagemLCD("Senha Incorreta!");
    atualizaDisplay();
    digitalWrite(pinoLedVerde, HIGH);
    digitalWrite(pinoLedVermelho, LOW);
    estadoBomba = 0;
  }
}

void loop() {

  atualizaDisplay();

  exibirMensagemLCD("Digite a senha:");

  dadosTeclado();

  verificacaoSenha();
}

void exibirMensagemLCD(String mensagem) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mensagem);
}
void exibirMensagemLCD2(String mensagem) {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print(mensagem);
  delay(2000);
}
void exibirMensagemLCD3(String mensagem) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mensagem);
  delay(2000);
}
//Codigo teclado funcionando 100%
char getKey() {
  char key = NO_KEY;
  for (int linha = 0; linha < numLinhas; linha++) {
    pinMode(rowPins[linha], OUTPUT);
    digitalWrite(rowPins[linha], LOW);
    for (int coluna = 0; coluna < numColunas; coluna++) {
      pinMode(colPins[coluna], INPUT_PULLUP);
      if (digitalRead(colPins[coluna]) == LOW) {
        delay(50);
        if (digitalRead(colPins[coluna]) == LOW) {
          key = keymap[linha][coluna];
        }
      }
    }
    digitalWrite(rowPins[linha], HIGH);
  }
  return key;
}
