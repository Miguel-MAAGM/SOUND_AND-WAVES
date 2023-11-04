#ifndef _TOWER_CONTROL_H
#define _TOWER_CONTROL_H
#include "Arduino.h"
#include "utils.h"
#include <ESP_FlexyStepper.h>

// IO pin assignments
#define MOTOR_1_STEP_PIN 33
#define MOTOR_1_DIRECTION_PIN 25

#define MOTOR_2_STEP_PIN 27
#define MOTOR_2_DIRECTION_PIN 14

#define MOTOR_3_STEP_PIN 27
#define MOTOR_3_DIRECTION_PIN 14

#define EMERGENCY_STOP_PIN 13 // define the IO pin the emergency stop switch is connected to
#define LIMIT_SWITCH_PIN 32   // define the IO pin where the limit switches are connected to (switches in series in normally closed setup against ground)

// Speed settings
#define DISTANCE_TO_TRAVEL_IN_STEPS 100
#define SPEED_IN_STEPS_PER_SECOND 100
#define ACCELERATION_IN_STEPS_PER_SECOND 800
#define DECELERATION_IN_STEPS_PER_SECOND 800



class MotorController
{

public:
    MotorController(const char *TagONE,const char *TagTWO,const char *TagTHREE);
    void Setting_MotorONE(MotorSettings Settings);
    void Setting_MotorSecond(MotorSettings Settings);
    void Setting_MotorThree(MotorSettings Settings);
    void moveMotorOne(int steps);
    void moveMotorSecond(int steps);
    void moveMotorThree(int steps);
    void moveToHome(int steps);
    
    
    bool init();
    
    
    
    MotorSettings getMotorOneSet();
    MotorSettings getMotorSecondSet();
    MotorSettings getMotorThreeSet();


private:
    ESP_FlexyStepper MTOne;
    ESP_FlexyStepper MTSecond;
    ESP_FlexyStepper MTThree;
    MotorSettings MotorOne;
    MotorSettings MotorSecond;
    MotorSettings MotorThree;

};
#endif