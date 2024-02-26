#include "towerControl.h"
MotorController::MotorController(const char *TagONE,const char *TagTWO,const char *TagTHREE):MotorOne(TagONE),MotorSecond(TagTWO),MotorThree(TagTHREE)
{
    // Inicializa los motores paso a paso aquí
    MTViela.connectToPins(MOTOR_V_STEP_PIN, MOTOR_V_DIRECTION_PIN);
    MTOne.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
    MTSecond.connectToPins(MOTOR_Y_STEP_PIN, MOTOR_Y_DIRECTION_PIN); // Ejemplo de configuración de pines para MTSecond

    // set the speed and acceleration rates for the stepper motor
    MTViela.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
    MTViela.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
    MTViela.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

    MTOne.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
    MTOne.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
    MTOne.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

    MTSecond.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
    MTSecond.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
    MTSecond.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

    // Not start the stepper instance as a service in the "background" as a separate task
    // and the OS of the ESP will take care of invoking the processMovement() task regularily so you can do whatever you want in the loop function

    
}


void MotorController::Setting_Motor(ESP_FlexyStepper *MT,MotorSettings Settings)
{
    MT->setSpeedInMillimetersPerSecond(Settings.MMSpeedPerSecond);
    MT->setAccelerationInStepsPerSecondPerSecond(Settings.AcclPerSecond);
    MT->setDecelerationInStepsPerSecondPerSecond(Settings.DcltPerSecond);
    MT->setStepsPerMillimeter(Settings.StepPerMM);
    MT->setStepsPerRevolution(Settings.StepPerMM);
}

void MotorController::moveMotor(int stepsA,int stepsB,int stepsC)
{
    // Mueve MTViela con la cantidad de pasos especificada
    MTViela.setTargetPositionRelativeInSteps(stepsA);
    MTOne.setTargetPositionRelativeInSteps(stepsB);
    MTSecond.setTargetPositionRelativeInSteps(stepsC);

}

void MotorController::moveToHome()
{
    MTOne.moveToHomeInMillimeters(-1,100,1500,LIMIT_SWITCH_PIN_A);
    MTSecond.moveToHomeInMillimeters(-1,100,1500,LIMIT_SWITCH_PIN_B);
}

bool MotorController::init()
{

   // MotorOne.loadFromFlash();
   // MotorSecond.loadFromFlash();
  ///  MotorThree.loadFromFlash();
   /// Setting_Motor(&MTViela,MotorOne);
   // Setting_Motor(&MTOne,MotorSecond);
   // Setting_Motor(&MTSecond,MotorThree);

    MTViela.startAsService(1);
    MTOne.startAsService(1);
    MTSecond.startAsService(1);
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
