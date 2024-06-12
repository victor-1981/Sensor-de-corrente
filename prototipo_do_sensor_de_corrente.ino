#include "EmonLib.h"

EnergyMonitor emon1; 

const float tensao = 127.0;  // Tensão da rede elétrica (em Volts)
const float correnteMinima = 1;  // Corrente mínima para considerar consumo (em Amperes)
float energiaConsumida = 0.0;  // Energia consumida (em Wh)
unsigned long ultimoTempo = 0;  // Variável para armazenar o último tempo em milissegundos

void setup() {
  Serial.begin(9600);  // Inicialize a comunicação serial
  emon1.current(A0, 60.6);  // Corrente: pino de entrada A0, calibração para SCT-013-000
  ultimoTempo = millis();  // Inicialize o tempo
}

void loop() {
  double Irms = emon1.calcIrms(1480);  // Calcule a corrente (Irms)
  
  if (Irms < correnteMinima) {
    Irms = 0.0;  // Ignore leituras de corrente muito baixas
  }
  
  double potencia = Irms * tensao;  // Calcule a potência (em Watts)
  
  // Calcule o tempo decorrido desde a última leitura (em horas)
  unsigned long tempoAtual = millis();
  float tempoDecorrido = (tempoAtual - ultimoTempo) / 3600000.0;  // Tempo em horas
  ultimoTempo = tempoAtual;  // Atualize o último tempo

  // Calcule a energia consumida durante o intervalo de tempo
  energiaConsumida += potencia * tempoDecorrido;  // Energia em Wh
  
  // Exiba os valores no monitor serial
  Serial.println("=================================");
  Serial.print("Corrente: ");
  Serial.print(Irms);
  Serial.println(" A");

  Serial.print("Potência: ");
  Serial.print(potencia);
  Serial.println(" W");

  Serial.print("Energia Consumida: ");
  Serial.print(energiaConsumida);
  Serial.println(" Wh");
  Serial.println("=================================");

  delay(1000);  // Aguarde 1 segundo antes da próxima leitura
}
