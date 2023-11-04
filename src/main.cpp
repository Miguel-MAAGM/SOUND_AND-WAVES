#include "Arduino.h"
#include "towerControl.h"
#include "WifiManager.h"
#include "esp_task_wdt.h"  // Para trabajar con el WDT en las tareas
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  // Para trabajar con tareas en FreeRTOS

const char *ssid = "TP-Link_D274";       // Enter SSID
const char *password = "47673922";       // Enter Password
const char *server_ip = "192.168.1.113"; // Enter server adress
const uint16_t server_port = 12345;      // Enter server port

 MotorController MotorCtrl("M1","M2","M3");
 WifiManager Wifi_Manager(ssid,password,server_ip,server_port);
 
void setup()
{
  Serial.begin(9600);
  
  for (size_t i = 0; i < 100; i++)
  {
    Serial.print(".");
    delay(2);
  }

  Wifi_Manager.connectToWifi();
  Wifi_Manager.connectToServer();
  MotorCtrl.init();
 

}

void loop()
{
  Wifi_Manager.checkForData();
}
// just move the stepper back and forth in an endless loop
/* */

// Notice that you can now do whatever you want in the loop function without the need to call processMovement().
// also you do not have to care if your loop processing times are too long.
