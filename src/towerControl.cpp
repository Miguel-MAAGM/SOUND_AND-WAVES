#include "towerControl.h"
MotorController::MotorController(const char *TagONE,const char *TagTWO,const char *TagTHREE):MotorOne(TagONE),MotorSecond(TagTWO),MotorThree(TagTHREE)
{
    // Inicializa los motores paso a paso aquí
    MTOne.connectToPins(MOTOR_1_STEP_PIN, MOTOR_1_DIRECTION_PIN);
    MTSecond.connectToPins(MOTOR_2_STEP_PIN, MOTOR_2_DIRECTION_PIN);
    MTThree.connectToPins(MOTOR_3_STEP_PIN, MOTOR_3_DIRECTION_PIN); // Ejemplo de configuración de pines para MTThree

    // set the speed and acceleration rates for the stepper motor
    MTOne.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
    MTOne.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
    MTOne.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

    MTSecond.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
    MTSecond.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
    MTSecond.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

    MTThree.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
    MTThree.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
    MTThree.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

    // Not start the stepper instance as a service in the "background" as a separate task
    // and the OS of the ESP will take care of invoking the processMovement() task regularily so you can do whatever you want in the loop function

    
}
void MotorController::Setting_MotorONE(MotorSettings Settings)
{

    MTOne.setSpeedInMillimetersPerSecond(Settings.MMSpeedPerSecond);
    MTOne.setAccelerationInStepsPerSecondPerSecond(Settings.AcclPerSecond);
    MTOne.setDecelerationInStepsPerSecondPerSecond(Settings.DcltPerSecond);
    MTOne.setStepsPerMillimeter(Settings.StepPerMM);
    MTOne.setStepsPerRevolution(Settings.StepPerMM);
    

}
void MotorController::Setting_MotorSecond(MotorSettings Settings)
{
    MTSecond.setSpeedInMillimetersPerSecond(Settings.MMSpeedPerSecond);
    MTSecond.setAccelerationInStepsPerSecondPerSecond(Settings.AcclPerSecond);
    MTSecond.setDecelerationInStepsPerSecondPerSecond(Settings.DcltPerSecond);
    MTSecond.setStepsPerMillimeter(Settings.StepPerMM);
    MTSecond.setStepsPerRevolution(Settings.StepPerMM);
}
void MotorController::Setting_MotorThree(MotorSettings Settings)
{
    MTThree.setSpeedInMillimetersPerSecond(Settings.MMSpeedPerSecond);
    MTThree.setAccelerationInStepsPerSecondPerSecond(Settings.AcclPerSecond);
    MTThree.setDecelerationInStepsPerSecondPerSecond(Settings.DcltPerSecond);
    MTThree.setStepsPerMillimeter(Settings.StepPerMM);
    MTThree.setStepsPerRevolution(Settings.StepPerMM);
}
void MotorController::moveMotorOne(int steps)
{
    // Mueve MTOne con la cantidad de pasos especificada
    MTOne.moveToPositionInSteps(steps);
}

void MotorController::moveMotorSecond(int steps)
{
    // Mueve MTSecond con la cantidad de pasos especificada
    MTSecond.moveToPositionInSteps(steps);
}

void MotorController::moveMotorThree(int steps)
{
    // Mueve MTThree con la cantidad de pasos especificada
    MTThree.moveToPositionInSteps(steps);
}

void MotorController::moveToHome(int steps)
{
    MTOne.moveToHomeInMillimeters(1,100,1500,15);
}

bool MotorController::init()
{
    MotorOne.loadFromFlash();
    MotorSecond.loadFromFlash();
    MotorThree.loadFromFlash();
    Setting_MotorONE(MotorOne);
    Setting_MotorSecond(MotorSecond);
    Setting_MotorThree(MotorThree);

    MTOne.startAsService(1);
    MTSecond.startAsService(1);
    MTThree.startAsService(1);
    return true;
}   

MotorSettings MotorController::getMotorOneSet()
{
    return MotorOne;
}

MotorSettings MotorController::getMotorSecondSet()
{
    return MotorSecond;
}

MotorSettings MotorController::getMotorThreeSet()
{
    return MotorThree;
}
