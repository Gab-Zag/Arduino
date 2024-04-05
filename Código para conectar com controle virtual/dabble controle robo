#include <DabbleESP32.h>

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#define SENTIDO_MOTOR_ESQUERDO 27
#define VELOCIDADE_MOTOR_ESQUERDO 26

#define SENTIDO_MOTOR_DIREITO 25
#define VELOCIDADE_MOTOR_DIREITO 33

#define PINO_ARMA 32

const float toleranciaAnalogico = 0.5;
const int maxY = 7, minY = -6;
const int maxX = 6, minX = -7;

bool roboLigado;

void Desligando_o_Robo() {
  digitalWrite(PINO_ARMA, LOW);
  digitalWrite(SENTIDO_MOTOR_DIREITO, LOW);
  digitalWrite(SENTIDO_MOTOR_ESQUERDO, LOW);
  digitalWrite(VELOCIDADE_MOTOR_DIREITO, 0);
  digitalWrite(VELOCIDADE_MOTOR_ESQUERDO, 0);

  roboLigado = false;

  sentidoX = parado;
  sentidoY = parado;
}

void Inputs_do_Controle() {
  Dabble.processInput();
  if (GamePad.isStartPressed()) {  //Start para ligar o robo
    roboLigado = true;
    Serial.println("Robo Ligado");
  } else if (GamePad.isSelectPressed()) {  //Select para desligar o robo
    roboLigado = false;
    Serial.println("Robo Desligado");
  }
  if (roboLigado) {
    if (GamePad.isCrossPressed()) {  //se precionou a tecla x desliga a arma
      digitalWrite(PINO_ARMA, LOW);
      Serial.println("Arma Desligada");
    } else if (GamePad.isTrianglePressed()) {  //se precionou a tecla TRIÂNGULO liga a arma
      digitalWrite(PINO_ARMA, HIGH);
      Serial.println("Arma Ligada");
    }

    float valorAnalogicoEsquerdo = GamePad.getXaxisData();
    float valorAnalogicoDireito = -GamePad.getYaxisData();

    int pwmMotorDireito = 0;
    int pwmMotorEsquerdo = 0;

    if (valorAnalogicoDireito > toleranciaAnalogico) {  //Ir para frente
      digitalWrite(SENTIDO_MOTOR_DIREITO, HIGH);
      digitalWrite(SENTIDO_MOTOR_ESQUERDO, HIGH);

      if (valorAnalogicoEsquerdo > toleranciaAnalogico) {
        pwmMotorDireito = map(valorAnalogicoDireito - valorAnalogicoEsquerdo, minX, maxY, 255, 0);
        pwmMotorEsquerdo = map(valorAnalogicoDireito + valorAnalogicoEsquerdo, 0, maxX+maxY, 255, 0);
      }

      else if (valorAnalogicoEsquerdo < -toleranciaAnalogico) {
        pwmMotorDireito = map(valorAnalogicoDireito - valorAnalogicoEsquerdo, 0, maxX+maxY, 255, 0);
        pwmMotorEsquerdo = map(valorAnalogicoDireito + valorAnalogicoEsquerdo, minX, maxY, 255, 0);
      }

      else {
        pwmMotorDireito = map(valorAnalogicoDireito, toleranciaAnalogico, maxY, 255, 0);
        pwmMotorEsquerdo = map(valorAnalogicoDireito, toleranciaAnalogico, maxY, 255, 0);
      }
    }

    else if (valorAnalogicoDireito < -toleranciaAnalogico) {  // Ir para tras

      digitalWrite(SENTIDO_MOTOR_DIREITO, LOW);
      digitalWrite(SENTIDO_MOTOR_ESQUERDO, LOW);

      if (valorAnalogicoEsquerdo > toleranciaAnalogico) {
        pwmMotorDireito = map(valorAnalogicoEsquerdo + valorAnalogicoDireito, minX, maxY, 0, 255);
        pwmMotorEsquerdo = map(valorAnalogicoEsquerdo - valorAnalogicoDireito, 0, maxX+maxY, 0, 255);
      }

      else if (valorAnalogicoEsquerdo < -toleranciaAnalogico) {
        pwmMotorDireito = map(valorAnalogicoEsquerdo + valorAnalogicoDireito, minX, maxY, 0, 255);
        pwmMotorEsquerdo = map(valorAnalogicoEsquerdo - valorAnalogicoEsquerdo, 0, maxX+maxY, 0, 255);
      }

      else {
        pwmMotorDireito = map(valorAnalogicoDireito, -toleranciaAnalogico, minX, 0, 255);
        pwmMotorEsquerdo = map(valorAnalogicoDireito, -toleranciaAnalogico, minX, 0, 255);
      }
    }

    else {

      if (valorAnalogicoEsquerdo > toleranciaAnalogico) {

        digitalWrite(SENTIDO_MOTOR_DIREITO, LOW);
        digitalWrite(SENTIDO_MOTOR_ESQUERDO, HIGH);

        pwmMotorDireito = map(valorAnalogicoEsquerdo, toleranciaAnalogico, maxY, 0, 255);
        pwmMotorEsquerdo = map(valorAnalogicoEsquerdo, toleranciaAnalogico, maxY, 255, 0);
      }

      else if (valorAnalogicoEsquerdo < -toleranciaAnalogico) {

        digitalWrite(SENTIDO_MOTOR_DIREITO, HIGH);
        digitalWrite(SENTIDO_MOTOR_ESQUERDO, LOW);

        pwmMotorDireito = map(valorAnalogicoEsquerdo, -toleranciaAnalogico, minX, 255, 0);
        pwmMotorEsquerdo = map(valorAnalogicoEsquerdo, -toleranciaAnalogico, minX, 0, 255);
      }

      else {
        digitalWrite(SENTIDO_MOTOR_ESQUERDO, LOW);
        digitalWrite(SENTIDO_MOTOR_DIREITO, LOW);
      }
    }

    Serial.print("PWM Direito: ");
    Serial.println(pwmMotorDireito);
    Serial.print("PWM Esquerdo: ");
    Serial.println(pwmMotorEsquerdo);

    analogWrite(VELOCIDADE_MOTOR_DIREITO, pwmMotorDireito);
    analogWrite(VELOCIDADE_MOTOR_ESQUERDO, pwmMotorEsquerdo);
  } else {
    Desligando_o_Robo();
  }
}

void setup() {
  Serial.begin(115200);
  Dabble.begin("ESP_START_ROBOTICA");  //Nome do bluetooth da esp

  pinMode(SENTIDO_MOTOR_ESQUERDO, OUTPUT);
  pinMode(VELOCIDADE_MOTOR_DIREITO, OUTPUT);

  pinMode(SENTIDO_MOTOR_DIREITO, OUTPUT);
  pinMode(VELOCIDADE_MOTOR_DIREITO, OUTPUT);

  pinMode(PINO_ARMA, OUTPUT);
  Desligando_o_Robo();
}

void loop() {
  Dabble.processInput();
  Inputs_do_Controle();
}
