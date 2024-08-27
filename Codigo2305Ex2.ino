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

int pinoLedVermelho = 8;
int pinoLedVerde = 9;
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
}

unsigned long tempoAnterior = 0;
const unsigned long intervalo = 1000;

unsigned long taB = 0;
const unsigned ib1 = 1000;
const unsigned ib2 = 9000;

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
    beep();
    int displayTime = (c * 100) + i;
    display.showNumberDecEx(displayTime, 0b01000000, true);
    
  }
}

void beep(){
unsigned long tempoAtual = millis();
  if (tempoAtual - taB <= ib1 ) 
  {
  digitalWrite(pinoBuzzer, HIGH);
  }
  else{
    if (tempoAtual - taB <= ib2 ) 
    {
      digitalWrite(pinoBuzzer, LOW);
    }
    else
    {
      taB = tempoAtual;
    }
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
    
  }
}

void verificacaoSenha() {
  if (strcmp(entradaSenha, senhaCorreta) == 0) {
    atualizaDisplay();
    exibirMensagemLCD("Senha Correta!");
    exibirMensagemLCD3("Bomba Desarmada");
    digitalWrite(pinoLedVerde, LOW);
    digitalWrite(pinoLedVermelho, HIGH);
    estadoBomba = 1;
  } else {
    exibirMensagemLCD("Senha Incorreta!");
    atualizaDisplay();
    digitalWrite(pinoLedVerde, HIGH);
    digitalWrite(pinoLedVermelho, LOW);
    estadoBomba = 0;
    
    // Deduct 10 seconds from the remaining time
    if (c > 0 || (c == 0 && i >= 10)) {
      if (i >= 10) {
        i -= 10;
      } else {
        c--;
        i = 60 - (10 - i);
      }
    }
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
//Codigo para ler o teclado
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
