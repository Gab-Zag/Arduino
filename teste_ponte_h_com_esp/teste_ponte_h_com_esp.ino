#include <Bluepad32.h>

#define PINO_SENTIDO_M1 27
#define PINO_VELOCIDADE_M1 14

void setup() {
  Serial.begin(115200);
  pinMode(PINO_SENTIDO_M1, OUTPUT);
  digitalWrite(PINO_SENTIDO_M1, HIGH);
  pinMode(PINO_VELOCIDADE_M1, OUTPUT);
}

void loop() {
  for (int i = 0; i < 256; i++) {
    analogWrite(PINO_VELOCIDADE_M1, i);
    delay(100);
  }
}