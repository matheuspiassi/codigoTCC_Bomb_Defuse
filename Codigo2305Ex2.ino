#include <Wire.h>
#include <TM1637Display.h>
#include <LiquidCrystal_I2C.h>

const int numLinhas = 4;
const int numColunas = 4;
char keymap[numLinhas][numColunas] = {
  { '1', '4', '7', '1' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', '#' },
  { 'A', 'B', 'C', 'C' }
};
byte rowPins[numLinhas] = { 43, 45, 47, 49 };
byte colPins[numColunas] = { 35, 37, 39, 41 };

int pinoLedVermelho = 22;
int pinoLedVerde = 24;
int pinoBuzzer = 13;

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
  pinMode(pinoBuzzer, OUTPUT);
  i = 0;
  c = 5; //minutos
  display.setBrightness(7);
  digitalWrite(pinoLedVermelho, LOW);
  digitalWrite(pinoLedVerde, HIGH);
}

unsigned long tempoAnterior = 0;
const unsigned long intervalo = 1000;

void displayNumber(int number) {
  display.showNumberDec(number, true);
}

void atualizaDisplay() {
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoAnterior >= intervalo && estadoBomba == 0) {
    tempoAnterior = tempoAtual;

    if (i > 0) {
      i--;
    } else if (c > 0) {
      i = 59;
      c--;
    }
    int displayTime = (c * 100) + i;
    display.showNumberDecEx(displayTime, 0b01000000, true);
  }
}

void beep() {
  unsigned long tempoAtual = millis();

  if(c == 0 && i == 0){
    digitalWrite(pinoBuzzer, HIGH);
    return;
  }
  
  if ((tempoAtual - tempoAnterior) < 50) {
    digitalWrite(pinoBuzzer, HIGH);  
  } else {
    digitalWrite(pinoBuzzer, LOW);   
  }
}

void dadosTeclado() {
  int t = 0;
  while (t < 6) {
    char key = getKey();
    if (key != NO_KEY) {
      entradaSenha[t++] = key;
      lcd.setCursor(t - 1, 1);
      lcd.print(key);
    }
    delay(200);
    atualizaDisplay();
    beep();  
  }
}

void verificacaoSenha() {
  if (strcmp(entradaSenha, senhaCorreta) == 0) {
    atualizaDisplay();
    beep();  
    exibirMensagemLCD("Senha Correta!");
    exibirMensagemLCD3("Bomba Desarmada");
    digitalWrite(pinoLedVerde, LOW);
    digitalWrite(pinoLedVermelho, HIGH);
    estadoBomba = 1;
  } else {
    exibirMensagemLCD2("Incorreta  -25s");
    atualizaDisplay();
    beep(); 
    digitalWrite(pinoLedVerde, HIGH);
    digitalWrite(pinoLedVermelho, LOW);
    estadoBomba = 0;

    if (c > 0 || (c == 0 && i >= 25)) {
      if (i >= 25) {
        i -= 25;
      } else {
        c--;
        i = 60 - (25 - i);
      }
    }
  }
}

void loop() {
  atualizaDisplay();  
  beep();  
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
  lcd.setCursor(0, 0);
  lcd.print(mensagem);
  delay(2000);
}

void exibirMensagemLCD3(String mensagem) {
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print(mensagem);                                                                                        
  delay(2000);
}

// Código para ler o teclado
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
