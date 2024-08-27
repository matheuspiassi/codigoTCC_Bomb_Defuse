#include <TM1637Display.h>
// Configuração do display e pino de comunicação
#define CLK_PIN 2
#define DIO_PIN 3

// Cria uma instância do TM1637Display
TM1637Display display(CLK_PIN, DIO_PIN);

// Função para converter um número para exibição no display
void displayNumber(int number) {
  // Exibe o número no display
  display.showNumberDec(number, false);
}

void setup() {
  // Inicializa a comunicação com o display
  display.setBrightness(7);
}

void loop() {
  
  // Contagem regressiva de 30 segundos
  
    int counter = 30;
    while (counter >= 0) {
    // Exibe o número atual no display
    displayNumber(counter);

    // Aguarda 1 segundo
    delay(1000);

    // Decrementa o contador
    counter--;
  }
 }
 
