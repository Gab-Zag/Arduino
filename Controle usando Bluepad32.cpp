#include <Bluepad32.h>

#define PINO_SENTIDO_M1 33
#define PINO_VELOCIDADE_M1 25
#define PINO_SENTIDO_M2 32
#define PINO_VELOCIDADE_M2 26

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

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

      int medida = myController->axisRY();
      int medida2 = myController->axisX();

      Serial.println(medida);

      if (medida > 0) {
        digitalWrite(PINO_SENTIDO_M1, HIGH);
        digitalWrite(PINO_SENTIDO_M2, HIGH);
      }

      else {
        digitalWrite(PINO_SENTIDO_M1, LOW);
        digitalWrite(PINO_SENTIDO_M2, LOW);
      }

      int conversao = map(abs(medida), 0, 512, 0, 255);

      analogWrite(PINO_VELOCIDADE_M1, conversao);
      analogWrite(PINO_VELOCIDADE_M2, conversao);

      /* if(medida2 > 0){
    digitalWrite(PINO_SENTIDO_M2, HIGH);
  }else{
    digitalWrite(PINO_SENTIDO_M2, LOW);
  } */
    }

    else {
      Serial.println("Não foi possível ler o controle");
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
  pinMode(PINO_SENTIDO_M2, OUTPUT);
  pinMode(PINO_VELOCIDADE_M2, OUTPUT);
}

void loop() {
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();
}
