#include <Bluepad32.h>  //Biblioteca para o controle bluetooth

//rodas
#define PINO_SENTIDO_M1 25
#define PINO_VELOCIDADE_M1 26
#define PINO_SENTIDO_M2 27
#define PINO_VELOCIDADE_M2 14

//arma
#define PINO_SENTIDO_ARMA 12
#define PINO_VELOCIDADE_ARMA 13

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

bool roboLigado, armaLigada = false;

// Função para conctar o controle e garantir que somente um controle se conecte
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but could not found empty slot");
  }
}

//Função para Disconectar o controle
void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

  if (!foundController) {
    Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
  }
}

//Inputs do controle
void processControllers() {
  for (auto myController : myControllers) {

    if (myController && myController->isConnected() && myController->hasData() && myController->isGamepad()) {
      uint16_t botoesMisc = myController->miscButtons();

      if (botoesMisc == 0x04) {  //Start do controle para ligar o controle
        roboLigado = !roboLigado;
      }
      if (roboLigado) {

        uint16_t botoesPressionados = myController->buttons();

        if (botoesPressionados == 0x0008) {  // se pressionou apenas TRIÂNGULO

          //digitalWrite(PINO_SENTIDO_ARMA, !digitalRead(PINO_SENTIDO_ARMA));
          armaLigada = !armaLigada;
          if (armaLigada) {
            digitalWrite(PINO_VELOCIDADE_ARMA, HIGH);
          } else {
            digitalWrite(PINO_VELOCIDADE_ARMA, LOW);
          }
        }

        int16_t valorAnalogicoDireito = myController->axisX();
        int16_t valorAnalogicoEsquerdo = myController->axisX();

        int16_t valorAnalogicoTras = myController->axisRY();
        int16_t valorAnalogicoFrente = myController->axisRY();

        Serial.println(valorAnalogicoDireito);
        if (valorAnalogicoDireito == 0) {
          analogWrite(PINO_VELOCIDADE_M1, 0);
          analogWrite(PINO_VELOCIDADE_M2, 0);
        }
        //ir para frente
        if (valorAnalogicoFrente > 0) {  // Ir para frente
          digitalWrite(PINO_SENTIDO_M1, LOW);
          digitalWrite(PINO_SENTIDO_M2, LOW);
          int pwmMotorDireito = abs(valorAnalogicoFrente);
          int pwmMotorEsquerdo = abs(valorAnalogicoFrente);
          pwmMotorDireito = map(pwmMotorDireito, 0, 1024, 0, 255);
          pwmMotorEsquerdo = map(pwmMotorEsquerdo, 0, 1024, 0, 255);
          analogWrite(PINO_VELOCIDADE_M1, pwmMotorDireito);
          analogWrite(PINO_VELOCIDADE_M2, pwmMotorEsquerdo);
        }
        if (valorAnalogicoTras < 0) {  // Ir para trás
          digitalWrite(PINO_SENTIDO_M1, HIGH);
          digitalWrite(PINO_SENTIDO_M2, HIGH);
          int pwmMotorDireito = abs(valorAnalogicoTras);
          int pwmMotorEsquerdo = abs(valorAnalogicoTras);
          pwmMotorDireito = map(pwmMotorDireito, 0, 1024, 0, 255);
          pwmMotorEsquerdo = map(pwmMotorEsquerdo, 0, 1024, 0, 255);
          analogWrite(PINO_VELOCIDADE_M1, pwmMotorDireito);
          analogWrite(PINO_VELOCIDADE_M2, pwmMotorEsquerdo);
        }
        if (valorAnalogicoEsquerdo < 0) {  // Curva para a esquerda
          digitalWrite(PINO_VELOCIDADE_M1, HIGH);
          digitalWrite(PINO_VELOCIDADE_M2, LOW);
          int pwmMotorDireito = abs(valorAnalogicoDireito);
          int pwmMotorEsquerdo = abs(valorAnalogicoDireito);
          pwmMotorDireito = map(pwmMotorDireito, 0, 1024, 0, 255);
          pwmMotorEsquerdo = map(pwmMotorEsquerdo, 0, 1024, 0, 255);
          analogWrite(PINO_VELOCIDADE_M1, pwmMotorDireito);
          analogWrite(PINO_VELOCIDADE_M2, pwmMotorEsquerdo);
        }
        if (valorAnalogicoEsquerdo > 0) {  // Curva para a direita
          digitalWrite(PINO_VELOCIDADE_M1, LOW);
          digitalWrite(PINO_VELOCIDADE_M2, HIGH);
          int pwmMotorDireito = abs(valorAnalogicoDireito);
          int pwmMotorEsquerdo = abs(valorAnalogicoDireito);
          pwmMotorDireito = map(pwmMotorDireito, 0, 1024, 0, 255);
          pwmMotorEsquerdo = map(pwmMotorEsquerdo, 0, 1024, 0, 255);
          analogWrite(PINO_VELOCIDADE_M1, pwmMotorDireito);
          analogWrite(PINO_VELOCIDADE_M2, pwmMotorEsquerdo);
        }
      } else {
        analogWrite(PINO_VELOCIDADE_M1, 0);
        analogWrite(PINO_VELOCIDADE_M2, 0);
        digitalWrite(PINO_VELOCIDADE_ARMA, false);
      }
    } else {
      digitalWrite(PINO_SENTIDO_M1, 0);
      digitalWrite(PINO_SENTIDO_M2, 0);
      digitalWrite(PINO_VELOCIDADE_M1, 0);
      digitalWrite(PINO_VELOCIDADE_M2, 0);
      digitalWrite(PINO_VELOCIDADE_ARMA, 0);
    }
  }
}

void setup() {

  Serial.begin(115200);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);
  BP32.forgetBluetoothKeys();

  pinMode(PINO_SENTIDO_M1, OUTPUT);
  pinMode(PINO_VELOCIDADE_M1, OUTPUT);
  digitalWrite(PINO_VELOCIDADE_M1, LOW);

  pinMode(PINO_SENTIDO_M2, OUTPUT);
  pinMode(PINO_VELOCIDADE_M2, OUTPUT);
  digitalWrite(PINO_VELOCIDADE_M2, LOW);

  pinMode(PINO_SENTIDO_ARMA, OUTPUT);
  pinMode(PINO_VELOCIDADE_ARMA, OUTPUT);
  digitalWrite(PINO_VELOCIDADE_ARMA, LOW);

  roboLigado = false, armaLigada = false;
}

void loop() {
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();
}