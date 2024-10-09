#include <SevSeg.h>

// Classe para o Sistema de Gestão Inteligente de Temperatura
class SistemaTemperatura {
  private:
    const int pinoSensor;   // Pino onde está conectado o sensor LM35
    float temperaturaC;     // Variável para armazenar a temperatura em Celsius

  public:
    // Construtor para inicializar o pino do sensor
    SistemaTemperatura(int pino) : pinoSensor(pino), temperaturaC(0.0) {}

    // Método para ler a temperatura do sensor LM35
    float lerTemperatura() {
      int valorLido = analogRead(pinoSensor);  // Leitura do valor analógico do sensor
      float milivolts = (valorLido / 1024.0) * 5000;  // Converter o valor lido para milivolts
      return milivolts / 10.0;  // LM35 fornece 10mV por grau Celsius
    }
};

// Objeto do display de sete segmentos
SevSeg sevseg;

// Criar uma instância do sistema de temperatura
SistemaTemperatura sistemaTemp(A0);

// Definir pinos dos LEDs
const int pinoLED_Azul = 0;    // LED azul conectado ao pino 0
const int pinoLED_Vermelho = 1; // LED vermelho conectado ao pino 1

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};  // Pinos dos dígitos do display
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};  // Pinos dos segmentos do display
  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_CATHODE;  // Display de cátodo comum

  // Inicialização do display de 7 segmentos
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);  // Define o brilho do display

  // Inicializar os pinos dos LEDs
  pinMode(pinoLED_Azul, OUTPUT);
  pinMode(pinoLED_Vermelho, OUTPUT);

  // Apagar ambos os LEDs no início
  digitalWrite(pinoLED_Azul, LOW);
  digitalWrite(pinoLED_Vermelho, LOW);
}

void loop() {
  // Variáveis para armazenar a soma das leituras e o número de leituras
  static float somaTemperaturas = 0;
  static int numeroDeLeituras = 0;

  // Atualiza a cada 1 segundo
  static unsigned long timerLeitura = millis();
  if (millis() - timerLeitura >= 1000) { // A cada 1 segundo, faça uma leitura
    timerLeitura += 1000;

    // Ler a temperatura do sensor LM35 e adicionar à soma
    float tempAtual = sistemaTemp.lerTemperatura();
    somaTemperaturas += tempAtual;
    numeroDeLeituras++;

    // Se 10 leituras foram realizadas (10 segundos), calcule a média
    if (numeroDeLeituras == 10) {
      float mediaTemperatura = somaTemperaturas / numeroDeLeituras;

      // Multiplicar por 100 para exibir com duas casas decimais
      int mediaTemperatura_int = (int)(mediaTemperatura * 100);

      // Exibir a média no display
      sevseg.setNumber(mediaTemperatura_int, 2);  // Exibir temperatura média com duas casas decimais

      // Acender o LED de acordo com a média da temperatura
      if (mediaTemperatura < 25.0) {
        digitalWrite(pinoLED_Azul, HIGH);    // Acende o LED azul
        digitalWrite(pinoLED_Vermelho, LOW); // Apaga o LED vermelho
      } else {
        digitalWrite(pinoLED_Azul, LOW);     // Apaga o LED azul
        digitalWrite(pinoLED_Vermelho, HIGH); // Acende o LED vermelho
      }

      // Resetar as variáveis para o próximo ciclo de 10 leituras
      somaTemperaturas = 0;
      numeroDeLeituras = 0;
    }
  }

  // Atualizar o display
  sevseg.refreshDisplay();
}

