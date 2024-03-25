#include "clientManege.h"
clientManege::clientManege(const char *server_ip, const uint16_t server_port)
{

    m_server_ip = server_ip;
    m_server_port = server_port;
}
clientManege::clientManege()
{
}
clientManege::~clientManege()
{
}

void clientManege::setServerParams(const char *server_ip, const uint16_t server_port)
{
    m_server_ip = server_ip;
    m_server_port = server_port;
}

bool clientManege::connectToServer()
{
    if (client.connect(m_server_ip, m_server_port))
    {
        Serial.println("Conectado al servidor");
        StaticJsonDocument<200> jsonDoc; // Tamaño máximo del JSON
        jsonDoc["name"] = ID;
        jsonDoc["type"] = "ACT";

        // Serializa el objeto JSON a una cadena
        String jsonStr;
        serializeJson(jsonDoc, jsonStr);
        client.println(jsonStr);
        return true;
    }
    else
    {
        Serial.println("Error al conectar al servidor.");
        return false;
    }
}

bool clientManege::checkForData()
{

    if (client.connected())
    {
        if (client.available())
        {
            char c = client.read();
            if (c == '\n')
            {
                // Procesar la línea completa almacenada en el buffer
                Serial.println("Línea completa recibida: " + buffer);
                buffer_f=buffer;
                // Limpia el buffer después de procesar la línea
                buffer = "";
                return true;
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

    return false;
}

void clientManege::sendToclient(String data)
{
    
  if (client.connected())
    {
      client.println(data);
       
       
    }
    else
    {
        Serial.println("SE CERRÓ, INTENTANDO CONECTAR");
        connectToServer();
    }

}

String clientManege::getCMD()
{
    return doc["CMD"];
}

String clientManege::getCfullCMD()
{
    return buffer_f;
}
