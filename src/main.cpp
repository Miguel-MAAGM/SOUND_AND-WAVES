#include "Arduino.h"
// #include "towerControl.h"
#include "WifiManager.h"
#include "esp_task_wdt.h" // Para trabajar con el WDT en las tareas
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" // Para trabajar con tareas en FreeRTOS
#include <ESP_FlexyStepper.h>
#include "towerControl.h"
const char *ssid = "celuMiguel";          // Enter SSID
const char *password = "12345678";        // Enter Password
const char *server_ip = "192.168.100.17"; // Enter server adress
const uint16_t server_port = 12345;       // Enter server port

MotorController MotorCtrl("M1","M2","M3");
WifiManager Wifi_Manager(ssid, password, server_ip, server_port);

int previousDirection = 1;
bool processJsonCommand(String jsonString);

void setup()
{
  pinMode(LIMIT_SWITCH_PIN_A, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_PIN_B, INPUT_PULLUP);
  Serial.begin(9600);
  // connect and configure the stepper motor to its IO pins
  MotorCtrl.init();
 
  MotorCtrl.moveToHome();
  
  Wifi_Manager.connectToWifi();

  Wifi_Manager.connectToServer();
  // stepper_x.moveToHomeInMillimeters(1,100,10000,LIMIT_SWITCH_PIN_A);
  // stepper_y.moveToHomeInMillimeters(1,100,10000,LIMIT_SWITCH_PIN_B);
}

void loop()
{

  if (Wifi_Manager.checkForData())
  {
    processJsonCommand(Wifi_Manager.getCfullCMD());
  }


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
    MotorCtrl.moveMotor(MTV,MT1,MT2);
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