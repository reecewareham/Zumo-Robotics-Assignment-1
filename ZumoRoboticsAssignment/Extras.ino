#include <Zumo32U4.h>
#include "src/TurnSensor/TurnSensor.h"
#include "src/PIDController/PIDController.h"

#define UP_BOUND_ROT 1.0
#define LOW_BOUND_ROT -1.0

float wrapAngle(float angle) {
  
  angle = fmod(angle + 180, 360);
  
  if (angle < 0) {
    
    angle += 360;
    
  }
  
  return (angle - 180);
  
}

//////////////////////////////////////////////////////////////////////////////////
/*
  Function that makes sure that the value is in between the lower bound and the 
  upper bound.
*/
//////////////////////////////////////////////////////////////////////////////////

bool inRange(float value, float lowerBound, float upperBound) {
  
  return ((value > lowerBound) && (value < upperBound));
  
}

//////////////////////////////////////////////////////////////////////////////////
/*
  Rotates perfect 90 degree turns using the PID controller and gyro.
*/
//////////////////////////////////////////////////////////////////////////////////

bool rotateAngle(float angle, int topSpeed) {

  PIDController rotationPID = PIDController((float)6.5, (float) 0.001, (float)0.0);

  float error, motorSpeed, targetAngle, currentHeading;
  bool continueLoop = true;

  turnSensorReset();
  turnSensorUpdate();

  currentHeading = getHeading();

  targetAngle = wrapAngle(currentHeading + angle);

  while(continueLoop) {
    
    turnSensorUpdate();
    error = wrapAngle(getHeading() - targetAngle);
    motorSpeed = rotationPID.calculate(fabs(error));

    continueLoop = !(inRange(fabs(error), LOW_BOUND_ROT, UP_BOUND_ROT));

    if(false == continueLoop) {
      
      motorSpeed = 0;
      
    }

    if (motorSpeed > topSpeed) {
      
      motorSpeed = topSpeed;
      
    }

    if (error > 0) {
      
      motors.setSpeeds(motorSpeed, -motorSpeed);
      
    } else {
      
      motors.setSpeeds(-motorSpeed, motorSpeed);
      
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////
/*
  Simple function that returns a readable angle value.
*/
//////////////////////////////////////////////////////////////////////////////////

int32_t getAngle() {
  
  return (((int32_t)turnAngle >> 16) *360) >> 16;
  
}

//////////////////////////////////////////////////////////////////////////////////
/*
  Puts the readings from the line sensors into a buffer which is then formatted
  properly using the sprintf function. This is then printed in Serial1. This is
  used for debugging.
*/
//////////////////////////////////////////////////////////////////////////////////

void printReadingsToSerial()
{
  
  static char buffer[80];
  sprintf(buffer, "%4d %4d %4d %4d %4d\n",
    lineSensorValues[0],
    lineSensorValues[1],
    lineSensorValues[2],
    lineSensorValues[3],
    lineSensorValues[4]
  );
  
  Serial1.print(buffer);
  
}


//////////////////////////////////////////////////////////////////////////////////
/*
  Calibrates line sensors by sweeping all line sensors over a black line multiple 
  times whilst taking a reading. This reading is used to determine the sensitivity 
  of the line sensors.
*/
//////////////////////////////////////////////////////////////////////////////////

void calibrateLineSensors()
{
  
  ledYellow(1);

  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-150, 150);
    }
    else
    {
      motors.setSpeeds(150, -150);
    }

    lineSensors.calibrate();
  }
  
  motors.setSpeeds(0, 0);

  ledYellow(0);
  
}
