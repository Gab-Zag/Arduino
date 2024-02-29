#include <Bluepad32.h>

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

void processControllers() {
  for (auto myController : myControllers) {

    if (myController && myController->isConnected() && myController->hasData() && myController->isGamepad()) {

      uint16_t botoesMisc = myController-> miscButtons();

      if (botoesMisc == 0x04)
        roboLigado = !roboLigado;

      if (roboLigado) {

        uint16_t botoesPressionados = myController->buttons();

        if (botoesPressionados == 0x0008) {  // se pressionou apenas X

          //digitalWrite(PINO_SENTIDO_ARMA, !digitalRead(PINO_SENTIDO_ARMA));
          armaLigada = !armaLigada;
          if (armaLigada){
            digitalWrite(PINO_VELOCIDADE_ARMA, HIGH);
          }
          else{
            digitalWrite(PINO_VELOCIDADE_ARMA, LOW);
          }
        }

        int16_t valorAnalogicoDireito = myController->axisRY();
        int16_t valorAnalogicoEsquerdo = myController->axisX();
        Serial.println (valorAnalogicoDireito);
        if (valorAnalogicoDireito == 0) {
          analogWrite(PINO_VELOCIDADE_M1, 0);
          analogWrite(PINO_VELOCIDADE_M2, 0);
        }

        else {

          if (valorAnalogicoDireito == 126) {
            digitalWrite(PINO_VELOCIDADE_M1, HIGH);
            digitalWrite(PINO_VELOCIDADE_M2, HIGH);
          }

          else {
            digitalWrite(PINO_VELOCIDADE_M1, LOW);
            digitalWrite(PINO_VELOCIDADE_M2, LOW);
          }

          int pwmMotorDireito = abs(valorAnalogicoDireito) - valorAnalogicoEsquerdo;
          int pwmMotorEsquerdo = abs(valorAnalogicoDireito) + valorAnalogicoEsquerdo;

          pwmMotorDireito = map(pwmMotorDireito, 0, 1024, 0, 255);
          pwmMotorEsquerdo = map(pwmMotorEsquerdo, 0, 1024, 0, 255);
          Serial.print (pwmMotorDireito);
          analogWrite(PINO_VELOCIDADE_M1, pwmMotorDireito);
          analogWrite(PINO_VELOCIDADE_M2, pwmMotorEsquerdo);
        }
      }

      else {
        analogWrite(PINO_VELOCIDADE_M1, 0);
        analogWrite(PINO_VELOCIDADE_M2, 0);
        digitalWrite(PINO_VELOCIDADE_ARMA, false);
      }
    } else Serial.println("");
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
