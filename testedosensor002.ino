//Breves alteraçoes
/*  Sensor de Pulso, teste eHealth Monitor
----------------------  Notas ----------------------  ----------------------
Este código:
1) Pisca um LED para o PIN 13 de pulsação ao vivo do usuário
2) Desvanece um LED para o PIN 5 do Live HeartBeat do usuário
3) Determina BPM
4) Imprime todos os itens acima em série

teste

 ----------------------       ----------------------  ----------------------
*/

#define PROCESSANDO_VISUALIZADOR 1
#define SERIAL_PLOTTER  2

//  Variaveis
int pulsePin = 0;                // Fio roxo do sensor de pulso conectado ao pino analógico 0
int blinkPin = 13;                // pino para piscar o led a cada batida
int fadePin = 5;                  // alfinete para fazer uma piscada elegante e elegante a cada batida
int fadeRate = 0;                // usado para acender o LED com PWM no fadePin

// Variáveis ​​Voláteis, usadas na rotina de serviço de interrupção!
volatile int BPM;                   // int que contém o analógico bruto em 0. atualizado a cada 2mS
volatile int Signal;                // mantém os dados brutos de entrada
volatile int IBI = 600;             // int que mantém o intervalo de tempo entre as batidas! Deve ser semeado!
volatile boolean Pulse = false;     // "True" quando a pulsação ao vivo do usuário é detectada. "Falso" quando não é uma "batida ao vivo".
volatile boolean QS = false;        // torna-se verdade quando Arduoino encontra uma batida.

// DEFINIR O TIPO DE SAÍDA SERIAL PARA SUAS NECESSIDADES
// PROCESSING_VISUALIZER funciona com o Pulse Sensor Processing Visualizer
// https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer
// SERIAL_PLOTTER emite dados do sensor para visualização com o Arduino Serial Plotter
// executa a Serial Plotter a 115200 baud: Tools/Serial Plotter ou Command+L
static int outputType = SERIAL_PLOTTER;


void setup(){
  pinMode(blinkPin, OUTPUT);         // pin que piscará conforme o seu batimento cardíaco!
  pinMode(fadePin, OUTPUT);          // alfinete que irá desaparecer com o seu batimento cardíaco!
  Serial.begin(115200);             // concordamos em falar rápido!
  interruptSetup();                 // configura para ler o sinal do Sensor de Pulso a cada 2mS
   // SE VOCÊ ESTIVER ALIMENTANDO O Sensor de Pulso COM TENSÃO MENOS DO QUE A TENSÃO DA PLACA,
   // NÃO COMENTE A PRÓXIMA LINHA E APLIQUE ESSA TENSÃO AO PIN A-REF
//   analogReference(EXTERNAL);
}


//  Onde a mágica acontece
void loop(){

    serialOutput() ;

  if (QS == true){     // Um ​​batimento cardíaco foi encontrado
                       // BPM e IBI foram determinados
                       // Quantified Self "QS" true quando o arduino encontra uma pulsação
        fadeRate = 255;         // Faz o efeito de fade do LED acontecer
                                // Defina a variável 'fadeRate' para 255 para desvanecer o LED com pulso
        serialOutputWhenBeatHappens();   // A Beat Happened, Output isso para serial.
        QS = false;                     // redefine o sinalizador Quantified Self para a próxima vez
  }

  ledFadeToBeat();                      // Faz o efeito de fade do LED acontecer
  delay(20);                             //  dar um tempo
}





void ledFadeToBeat(){
    fadeRate -= 15;                         // define o valor de fade do LED
    fadeRate = constrain(fadeRate,0,255);   // impede que o valor de fade do LED fique em números negativos!
    analogWrite(fadePin,fadeRate);          //  LED de desvanencimento
  }