#include "Arduino.h"
#include "clientManege.h"
#include "esp_task_wdt.h" // Para trabajar con el WDT en las tareas
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" // Para trabajar con tareas en FreeRTOS
#include <ESP_FlexyStepper.h>
#include <AccelStepper.h>
#include "WiFiManager.h"
#include "configAPP.h"
// #include "towerControl.h"
const uint16_t server_port = 12345;       // Enter server port

#define dirPin 12
#define stepPin 14
#define motorInterfaceType 1

// MotorController MotorCtrl("M1","M2","M3");

int previousDirection = 1;
bool processJsonCommand(String jsonString);
ESP_FlexyStepper stepper_x;
ESP_FlexyStepper stepper_y;
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin); // Piston
char ip_addrs[34];
clientManege Wifi_Manager;

void setup()
{

  Serial.begin(9600);
  WiFiManagerParameter custom_api_HostIP("apikey", "Host IP", "None", 34);
  WiFiManager wm;  
  wm.addParameter(&custom_api_HostIP);
  wm.setTimeout(300);

  if (!wm.autoConnect("Tower_1")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }
  strcpy(ip_addrs, custom_api_HostIP.getValue());

   if (strcmp(ip_addrs, "None") == 0) {
    Serial.println(ip_addrs);
    ipHost=recuperarIP();
    Serial.println(ipHost);

    if(ipHost=="None"){
      Serial.println("ERROR from read IP");
    }
  }else{
    ipHost=String(ip_addrs);
    guardarIP(ipHost);
  }


  Serial.println(ipHost);

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

  stepper.setMaxSpeed(1000000); // Maxima Velocidad
  stepper.setAcceleration(500000);

    
  Wifi_Manager.setServerParams(ipHost.c_str(),server_port);
  if(!Wifi_Manager.connectToServer()){
    Serial.println("No conected");
    delay(5000);
    wm.resetSettings();
    ESP.restart();
  }
  
  
  // Maximo = 9000 Minimo=1000 Configuracion Driver 3600 y 2.8 [A]

  // stepper_x.moveToHomeInMillimeters(1,100,10000,LIMIT_SWITCH_PIN_A);
  // stepper_y.moveToHomeInMillimeters(1,100,10000,LIMIT_SWITCH_PIN_B);
  stepper.setSpeed(0);
  stepper.runSpeed();
  stepper_x.startAsService(1);
  stepper_y.startAsService(1);

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
    StaticJsonDocument<500> doc;
    doc["ID"] = ID;
    doc["CMD"] = "RES";
    doc["type"] = "ACT";
    
    JsonObject m1 = doc.createNestedObject("M1");
    MotorOne.toJson(m1);
    JsonObject m2 = doc.createNestedObject("M2");
    MotorOne.toJson(m2);
    JsonObject mVI = doc.createNestedObject("MVI");
    MotorOne.toJson(mVI);
    String json;
    serializeJson(doc, json);
    Wifi_Manager.sendToclient(json);
    Serial.println(json);
    Serial.println("Ejecutando comando GET...");

    // Lleva a cabo las acciones necesarias para el comando "GET"
  }
  else if (strcmp(CMD, "SET") == 0)
  {
    // Acción para el comando "SET"
    
    if(doc.containsKey("M1")){
    JsonObject mtOne = doc["M1"];
    MotorOne.readFromJson(mtOne);
    }else{
      Serial.println("NO KEY");
    }
    if(doc.containsKey("M2")){
    JsonObject mtTWO = doc["M2"];
    MotorTwo.readFromJson(mtTWO);
    }else{
      Serial.println("NO KEY");
    }
    if(doc.containsKey("MVI")){
    JsonObject mtVI = doc["MVI"];
    MotorVI.readFromJson(mtVI);
    }else{
      Serial.println("NO KEY");
    }


    Serial.println("Ejecutando comando SET...");
    // Lleva a cabo las acciones necesarias para el comando "SET"
  }
  else if (strcmp(CMD, "MOV") == 0)
  {

    float MT1 = doc["M1"];
    float MT2 = doc["M2"];
    float MTV = doc["MV"];
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
        Wifi_Manager.sendToclient("data");
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