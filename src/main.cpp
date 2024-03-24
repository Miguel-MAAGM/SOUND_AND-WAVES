#include "Arduino.h"
#include "towerControl.h"
#include "WifiManager.h"
#include "esp_task_wdt.h" // Para trabajar con el WDT en las tareas
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" // Para trabajar con tareas en FreeRTOS
#include <ESP_FlexyStepper.h>
#include <AccelStepper.h>

// #include "towerControl.h"
const char *ssid = "celuMiguel";          // Enter SSID
const char *password = "12345678";        // Enter Password
const char *server_ip = "192.168.212.17"; // Enter server adress
const uint16_t server_port = 12345;       // Enter server port

#define dirPin 12
#define stepPin 14
#define motorInterfaceType 1

// MotorController MotorCtrl("M1","M2","M3");
WifiManager Wifi_Manager(ssid, password, server_ip, server_port);

int previousDirection = 1;
bool processJsonCommand(String jsonString);
ESP_FlexyStepper stepper_x;
ESP_FlexyStepper stepper_y;
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin); // Piston

void setup()
{

  Serial.begin(9600);
  // connect and configure the stepper motor to its IO pins
  // MotorCtrl.init();
  // pinMode(LIMIT_SWITCH_PIN_A, INPUT_PULLUP);
  // pinMode(LIMIT_SWITCH_PIN_B, INPUT_PULLUP);
  // connect and configure the stepper motor to its IO pins
  stepper_x.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
  stepper_y.connectToPins(MOTOR_Y_STEP_PIN, MOTOR_Y_DIRECTION_PIN);
  // set the speed and acceleration rates for the stepper motor
  stepper_x.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper_x.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper_x.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

  stepper_y.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper_y.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper_y.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

  // Not start the stepper instance as a service in the "background" as a separate task
  // and the OS of the ESP will take care of invoking the processMovement() task regularily so you can do whatever you want in the loop function
  stepper_x.startAsService(1);
  stepper_y.startAsService(1);

  Wifi_Manager.connectToWifi();
  stepper.setMaxSpeed(1000000); // Maxima Velocidad
  stepper.setAcceleration(500000);
  Wifi_Manager.connectToServer();
  // Maximo = 9000 Minimo=1000 Configuracion Driver 3600 y 2.8 [A]

  // stepper_x.moveToHomeInMillimeters(1,100,10000,LIMIT_SWITCH_PIN_A);
  // stepper_y.moveToHomeInMillimeters(1,100,10000,LIMIT_SWITCH_PIN_B);
  stepper.setSpeed(0);
  stepper.runSpeed();
}

void loop()
{

  if (Wifi_Manager.checkForData())
  {

    processJsonCommand(Wifi_Manager.getCfullCMD());
  }
  // Aceleracion
  stepper.runSpeed();

  // Notice that you can now do whatever you want in the loop function without the need to call processMovement().
  // also you do not have to care if your loop processing times are too long.
}

bool processJsonCommand(String jsonString)
{
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error)
  {
    Serial.print("Error al analizar JSON: ");
    Serial.println(error.c_str());
    return false; // Devuelve false si hubo un error en el análisis
  }

  const char *CMD = doc["CMD"];

  if (strcmp(CMD, "GET") == 0)
  {
    // Acción para el comando "GET"

    Serial.println("Ejecutando comando GET...");

    // Lleva a cabo las acciones necesarias para el comando "GET"
  }
  else if (strcmp(CMD, "SET") == 0)
  {
    // Acción para el comando "SET"
    Serial.println("Ejecutando comando SET...");
    // Lleva a cabo las acciones necesarias para el comando "SET"
  }
  else if (strcmp(CMD, "MOV") == 0)
  {

    int MT1 = doc["M1"];
    int MT2 = doc["M2"];
    int MTV = doc["MV"];
    Serial.print("MT1: ");
    Serial.println(MT1);

    Serial.print("MT2: ");
    Serial.println(MT2);

    Serial.print("MTV: ");
    Serial.println(MTV);
    stepper.setSpeed(MTV);

    stepper_x.setTargetPositionRelativeInSteps(MT1);
    stepper_y.setTargetPositionRelativeInSteps(MT2);
    Serial.println("Ejecutando comando MOV...");
    // Lleva a cabo las acciones necesarias para el comando "MOV"
  }
  else if (strcmp(CMD, "STOP") == 0)
  {
    // Acción para el comando "STOP"
    Serial.println("Ejecutando comando STOP...");
    // Lleva a cabo las acciones necesarias para el comando "STOP"
  }
  else
  {
    Serial.println("Comando desconocido");
    return false; // Devuelve false si el comando es desconocido
  }

  return true; // Devuelve true si el proceso se realizó con éxito
}