#ifndef _TOWER_CONTROL_H
#define _TOWER_CONTROL_H
#include "Arduino.h"
#include "utils.h"
#include <ESP_FlexyStepper.h>

// IO pin assignments
const int MOTOR_V_STEP_PIN = 14;
const int MOTOR_V_DIRECTION_PIN = 12;

const int MOTOR_X_STEP_PIN = 25;
const int MOTOR_X_DIRECTION_PIN = 26;

const int MOTOR_Y_STEP_PIN = 32;
const int MOTOR_Y_DIRECTION_PIN = 33;

const int LIMIT_SWITCH_PIN_A = 15; // define the IO pin the emergency stop switch is connected to
const int LIMIT_SWITCH_PIN_B = 4;  // define the IO pin where the limit switches are connected to (switches in series in normally closed setup against ground)

// Speed settings
const int DISTANCE_TO_TRAVEL_IN_STEPS = 1000;
const int SPEED_IN_STEPS_PER_SECOND = 200;
const int ACCELERATION_IN_STEPS_PER_SECOND = 400;
const int DECELERATION_IN_STEPS_PER_SECOND = 400;



class MotorController
{

public:
    MotorController(const char *TagONE,const char *TagTWO,const char *TagTHREE);

    void moveMotor(int stepsA,int stepsB,int stepsC);

    void moveToHome();
    void Setting_Motor(ESP_FlexyStepper *MT,MotorSettings Settings);
   
    
    bool init();
    
    
    
    MotorSettings getMotorOneSet();
    MotorSettings getMotorSecondSet();
    MotorSettings getMotorThreeSet();
 
private:
    ESP_FlexyStepper MTViela;
    ESP_FlexyStepper MTOne;
    ESP_FlexyStepper MTSecond;
    MotorSettings MotorOne;
    MotorSettings MotorSecond;
    MotorSettings MotorThree;

};
#endif