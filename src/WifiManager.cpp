#include "WifiManager.h"

WifiManager::WifiManager(const char *ssid, const char *password,
                         const char *server_ip, const uint16_t server_port)
{
    m_ssid = ssid;
    m_password = password;
    m_server_ip = server_ip;
    m_server_port = server_port;
}

WifiManager::~WifiManager()
{
}

void WifiManager::connectToWifi()
{
    WiFi.begin(m_ssid, m_password);
    Serial.println("Conectando a la red WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Conexión WiFi exitosa.");
}

void WifiManager::connectToServer()
{
    if (client.connect(m_server_ip, m_server_port))
    {
        Serial.println("Conectado al servidor");
        StaticJsonDocument<200> jsonDoc; // Tamaño máximo del JSON
        jsonDoc["name"] = "mi_ID";
        jsonDoc["type"] = "mi_tipo";

        // Serializa el objeto JSON a una cadena
        String jsonStr;
        serializeJson(jsonDoc, jsonStr);
        client.println(jsonStr);

    }
    else
    {
        Serial.println("Error al conectar al servidor.");
    }
}

bool WifiManager::checkForData()
{

    if (client.connected())
    {
        if (client.available())
        {
            char c = client.read();
            if (c == -1)
            {
                Flag_ENDDATA = true;
                Serial.println("FIN TRANSMISIÓN");
            }

            if (c == '\n')
            {
                // Procesar la línea completa almacenada en el buffer
                Serial.println("Línea completa recibida: " + buffer);
                // Limpia el buffer después de procesar la línea
                buffer = "";
            }
            else
            {
                // Agregar el carácter al buffer
                buffer += c;
            }
        }
    }
    else
    {
        Serial.println("SE CERRÓ, INTENTANDO CONECTAR");
        connectToServer();
    }

    return true;
}
