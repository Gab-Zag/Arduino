#include <DabbleESP32.h>

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#define SENTIDO_MOTOR_ESQUERDO 27
#define VELOCIDADE_MOTOR_ESQUERDO 26

#define SENTIDO_MOTOR_DIREITO 25
#define VELOCIDADE_MOTOR_DIREITO 33

#define PINO_ARMA 32

int sentidoY; 
int sentidoX;
const int parado = 0;
const int horario = 1;
const int antihorario = 2;

bool roboLigado;

void Desligando_o_Robo(){
  digitalWrite(PINO_ARMA, LOW);
  digitalWrite(SENTIDO_MOTOR_DIREITO, LOW);
  digitalWrite(SENTIDO_MOTOR_ESQUERDO, LOW);
  digitalWrite(VELOCIDADE_MOTOR_DIREITO, 0);
  digitalWrite(VELOCIDADE_MOTOR_ESQUERDO, 0);

  roboLigado = false;

  sentidoX = parado; 
  sentidoY = parado;
}

void Inputs_do_Controle(){
  Dabble.processInput();
  if(GamePad.isStartPressed()){ //Start para ligar o robo
    roboLigado = true;
    Serial.println("Robo Ligado");
  }else if (GamePad.isSelectPressed()){ //Select para desligar o robo
    roboLigado = false;
    Serial.println("Robo Desligado");
  }
  if(roboLigado = true){
    if(GamePad.isCrossPressed()){ //se precionou a tecla x desliga a arma
      digitalWrite(PINO_ARMA, LOW);
      Serial.println("Arma Desligada");
    }else if(GamePad.isTrianglePressed()){ //se precionou a tecla TRIÂNGULO liga a arma
      digitalWrite(PINO_ARMA, HIGH);
      Serial.println("Arma Ligada");
    }
  }
}

void setup() {
  Serial.begin(115200);
  Dabble.begin("ESP_START_ROBOTICA"); //Nome do bluetooth da esp
  Desligando_o_Robo();
}

void loop() {
  Dabble.processInput();
  Inputs_do_Controle();
}
