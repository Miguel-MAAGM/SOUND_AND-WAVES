#ifndef _UTILS_H
#define _UTILS_H

#include <Preferences.h>
#include <ArduinoJson.h>

// #define DEBUG_
/*
#define DISTANCE_TO_TRAVEL_IN_STEPS  100
#define SPEED_IN_STEPS_PER_SECOND 100
#define ACCELERATION_IN_STEPS_PER_SECOND 800
#define DECELERATION_IN_STEPS_PER_SECOND 800*/
// Estructura para guardar en preferencias
struct MotorSettings
{
    const char *motorID; // ID que indica qué motor es
    float MMSpeedPerSecond = 100;
    float AcclPerSecond = 800;
    float DcltPerSecond = 800;
    float StepPerMM = 200;
    float StepPerRev = 200;
    MotorSettings(const char *ID)
    {
        motorID = ID;
    }
    // Función para cargar configuración desde la memoria flash
    bool loadFromFlash()
    {
#ifdef DEBUG_
        Serial.println(motorID);
#endif
        // Abre el espacio de preferencias
        Preferences preferences;
        if (!preferences.begin(motorID, false))
        {

            Serial.println("ERROR DE LECTURA");
            return false;
        }

        // Carga los datos desde preferencias
        if (preferences.getFloat("MPS", 0) != 0)
        {
#ifdef DEBUG_
            Serial.println("HAY DATOS");
#endif
            MMSpeedPerSecond = preferences.getFloat("MPS", 0.0);
            AcclPerSecond = preferences.getFloat("APS", 0.0);
            DcltPerSecond = preferences.getFloat("DPS", 0.0);
            StepPerMM = preferences.getFloat("SPM", 0.0);
            StepPerRev = preferences.getFloat("SPR", 0.0);
#ifdef DEBUG_
            Serial.print("LISTA DE CONFIGURACIONES ");
            Serial.println(motorID);
            Serial.print("MMSpeedPerSecond: ");
            Serial.println(MMSpeedPerSecond);
            Serial.print("AcclPerSecond: ");
            Serial.println(AcclPerSecond);
            Serial.print("DcltPerSecond: ");
            Serial.println(DcltPerSecond);
            Serial.print("StepPerMM: ");
            Serial.println(StepPerMM);
            Serial.print("StepPerRev: ");
            Serial.println(StepPerRev);
#endif
        }
        else
        {
#ifdef DEBUG_
            Serial.println("NO HAY DATOS");
#endif
            saveToFlash();
        }
        return true;
    }

    // Función para guardar configuración en la memoria flash
    bool saveToFlash()
    {
        // Abre el espacio de preferencias
        Preferences preferences;
        if (!preferences.begin(motorID, false))
        {

            Serial.println("ERROR DE LECTURA");
            return false;
        }
        // Guarda los datos en preferencias
        preferences.putFloat("MPS", MMSpeedPerSecond);
        preferences.putFloat("APS", AcclPerSecond);
        preferences.putFloat("DPS", DcltPerSecond);
        preferences.putFloat("SPM", StepPerMM);
        preferences.putFloat("SPR", StepPerRev);
        return true;
    }
    String serializeMotorSettingsToJson()
    {
        StaticJsonDocument<256> doc; // Tamaño en bytes del documento JSON (ajusta según tus necesidades)
        doc["motorID"] = motorID;
        doc["MMSpeedPerSecond"] = MMSpeedPerSecond;
        doc["AcclPerSecond"] = AcclPerSecond;
        doc["DcltPerSecond"] = DcltPerSecond;
        doc["StepPerMM"] = StepPerMM;
        doc["StepPerRev"] = StepPerRev;

        String jsonString;
        serializeJson(doc, jsonString);

        return jsonString;
    }
};

#endif