#include <Zumo32U4.h>
#include "TurnSensor.h"
#include "PIDController.h"

#define ROT_PROPOTION 6.5
#define ROT_INTEGRAL 0.001
#define ROT_DIRIVATIVE 0.0

#define UP_BOUND_ROT 1.0
#define LOW_BOUND_ROT -1.0

#define TRIG 2
#define ECHO 6
#define MAX_DISTANCE 30

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

float wrapAngle(float angle) {
  angle = fmod(angle + 180, 360);
  if (angle < 0) {
    angle += 360;
  }
  return (angle - 180);
}

bool inRange(float value, float lowerBound, float upperBound) {
  return ((value > lowerBound) && (value < upperBound));
}