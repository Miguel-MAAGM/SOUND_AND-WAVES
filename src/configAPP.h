#ifndef __configAPP_H__
#define __configAPP_H__
#include "Arduino.h"
#include "Preferences.h"
#include <ArduinoJson.h>

Preferences preferencias;
const int MOTOR_V_STEP_PIN = 14;
const int MOTOR_V_DIRECTION_PIN = 12;

const int MOTOR_X_STEP_PIN = 25;
const int MOTOR_X_DIRECTION_PIN = 26;

const int MOTOR_Y_STEP_PIN = 32;
const int MOTOR_Y_DIRECTION_PIN = 33;

const int LIMIT_SWITCH_PIN_A = 15; // define the IO pin the emergency stop switch is connected to
const int LIMIT_SWITCH_PIN_B = 4;  // define the IO pin where the limit switches are connected to (switches in series in normally closed setup against ground)

// Speed settings
const int Steps_Millimeter = 0;

const int DISTANCE_TO_TRAVEL_IN_STEPS = 1000;
const int SPEED_IN_STEPS_PER_SECOND = 50000;
const int ACCELERATION_IN_STEPS_PER_SECOND = 70000;
const int DECELERATION_IN_STEPS_PER_SECOND = 70000;
const int STEPS_PER_REVOLUTION = 200;
const int MAX_STEP = 450000;

struct motorParams
{
    float StepsPerMillimeter = Steps_Millimeter;
    float SpeedStepPerSecond = SPEED_IN_STEPS_PER_SECOND;
    float AccelSetpPerSecond = ACCELERATION_IN_STEPS_PER_SECOND;
    float DecelSetpPerSecond = DECELERATION_IN_STEPS_PER_SECOND;
    uint32_t StepPerRevolution = STEPS_PER_REVOLUTION;
    uint8_t base_dir = 0;
    uint8_t Home_dir = 0;
    const char *ID;

    // Constructor que acepta un ID
    motorParams(const char *id) : ID(id) {}

    // Método para guardar los parámetros en la memoria flash
    void savePreferences()
    {
        preferencias.begin(ID, false);

        preferencias.putFloat("SPM_", StepsPerMillimeter);
        preferencias.putFloat("SPS_", SpeedStepPerSecond);
        preferencias.putFloat("ASPS", AccelSetpPerSecond);
        preferencias.putFloat("DSPS", DecelSetpPerSecond);
        preferencias.putUInt("SPR_", StepPerRevolution);
        preferencias.putBool("bdir", base_dir);
        preferencias.putBool("Hdir", Home_dir);

        preferencias.end();
    }

    // Método para cargar los parámetros desde la memoria flash
    void loadPreferences()
    {
        preferencias.begin(ID, true); // Abrir en modo de solo lectura

        StepsPerMillimeter = preferencias.getFloat("SPM_", StepsPerMillimeter);
        SpeedStepPerSecond = preferencias.getFloat("SPS_", SpeedStepPerSecond);
        AccelSetpPerSecond = preferencias.getFloat("ASPS", AccelSetpPerSecond);
        DecelSetpPerSecond = preferencias.getFloat("DSPS", DecelSetpPerSecond);
        StepPerRevolution = preferencias.getUInt("SPR_", StepPerRevolution);
        base_dir = preferencias.getBool("bdir", base_dir);
        Home_dir = preferencias.getBool("Hdir", Home_dir);

        preferencias.end();
    }
    void readFromJson(JsonObject motorObj)
    {
        StepsPerMillimeter = motorObj["SPM_"].as<float>();
        SpeedStepPerSecond = motorObj["SPS_"].as<float>();
        AccelSetpPerSecond = motorObj["ASPS"].as<float>();
        DecelSetpPerSecond = motorObj["DSPS"].as<float>();
        StepPerRevolution = motorObj["SPR_"].as<uint>();
        base_dir = motorObj["bdir"].as<uint>();
        Home_dir = motorObj["Hdir"].as<uint>();
    }
    void toJson(JsonObject m1)
    {
        m1["SPM_"] = StepsPerMillimeter;
        m1["SPS_"] = SpeedStepPerSecond;
        m1["ASPS"] = AccelSetpPerSecond;
        m1["DSPS"] = DecelSetpPerSecond;
        m1["SPR_"] = StepPerRevolution;
        m1["bdir"] = base_dir;
        m1["Hdir"] = Home_dir;
    }
};

void printMotorParams(const motorParams &params)
{
    Serial.println("Motor Parameters:");
    Serial.print("Steps Per Millimeter: ");
    Serial.println(params.StepsPerMillimeter);
    Serial.print("Speed Steps Per Second: ");
    Serial.println(params.SpeedStepPerSecond);
    Serial.print("Acceleration Steps Per Second: ");
    Serial.println(params.AccelSetpPerSecond);
    Serial.print("Deceleration Steps Per Second: ");
    Serial.println(params.DecelSetpPerSecond);
    Serial.print("Steps Per Revolution: ");
    Serial.println(params.StepPerRevolution);
    Serial.print("Base Direction: ");
    Serial.println(params.base_dir);
    Serial.print("Home Direction: ");
    Serial.println(params.Home_dir);
}
void guardarIP(String ip_addrs)
{
    // Abre el espacio de preferencias
    preferencias.begin("netSet", false);

    // Guarda la dirección IP en la memoria flash
    preferencias.putString("ipAd", ip_addrs);

    // Cierra el espacio de preferencias
    preferencias.end();
}
String recuperarIP()
{
    // Abre el espacio de preferencias
    preferencias.begin("netSet", true);

    // Recupera la dirección IP de la memoria flash
    String ip_addrs = preferencias.getString("ipAd", "None");

    // Cierra el espacio de preferencias
    preferencias.end();

    return ip_addrs;
}
motorParams MotorOne("M_one");
motorParams MotorTwo("M_Two");
motorParams MotorVI("M_Vi");
String ipHost;
// float
// float
// float
// float

#endif