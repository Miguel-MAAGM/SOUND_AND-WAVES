#ifndef __WIFIMANAGER_H__
#define __WIFIMANAGER_H__
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
class WifiManager
{
private:
    const char *m_ssid;           // Enter SSID
    const char *m_password;       // Enter Password
    const char *m_server_ip;      // Enter server adress
    uint16_t m_server_port; // Enter server portpublic:
    WiFiClient client;
    bool Flag_ENDDATA=false;
    String buffer; // Buffer para acumular los datos
    String buffer_f;
public:
    WifiManager(const char *ssid, const char *password,
                const char *server_ip, const uint16_t server_port);
    ~WifiManager();
    void connectToWifi();
    void connectToServer();
    bool checkForData();
    String getCMD();
    StaticJsonDocument<200> doc;
    String getCfullCMD();


};

#endif // __WIFIMANAGER_H__