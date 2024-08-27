
#include <Arduino.h> //INCLUSÃO DE BIBLIOTECA
#include <TM1637Display.h> //INCLUSÃO DE BIBLIOTECA
 
#define CLK 3 //PINO DIGITAL UTILIZADO PELO CLK
#define DIO 2 //PINO DIGITAL UTILIZADO PELO DIO
#define TEST_DELAY 2000 //A QUANTIDADE DE TEMPO (EM MILISSEGUNDOS) ENTRE OS TESTES
 
TM1637Display display(CLK, DIO); //PPASSA OS PARÂMETROS PARA UMA FUNÇÃO DA BIBLIOTECA TM1637Display
 
const int pinoBotao = 4; //PINO DIGITAL UTILIZADO PELO BOTÃO
 
int contador = 0; //VARIÁVEL DO TIPO INTEIRA
int estadoPress = 0; //VARIÁVEL DO TIPO INTEIRA
 
void setup(){
  pinMode(pinoBotao,INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  //DO ARDUINO PARA GARANTIR QUE NÃO EXISTA FLUTUAÇÃO ENTRE 0 (LOW) E 1 (HIGH)
  display.setBrightness(0x0f); //DEFINE A INTESIDADE LUMINOSA DOS SEGMENTOS DO DISPLAY
}
 
void loop(){
   display.showNumberDec(contador,false); //IMPRIME NO DISPLAY A CONTAGEM ATUAL
   if(digitalRead(pinoBotao)== LOW){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
      if(estadoPress == 0){ //SE estadoPress FOR IGUAL A 0, FAZ
          contador++; //INCREMENTA VARIÁVEL EM +1
          estadoPress=1; //VARIÁVEL RECEBE 1
      }
   }else{ //SENÃO, FAZ
      estadoPress=0; //VARIÁVEL RECEBE 0
    }
}
