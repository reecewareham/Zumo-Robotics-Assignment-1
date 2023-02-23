#include <Zumo32U4.h>
#include "src/TurnSensor/TurnSensor.h"
#include "src/PIDController/PIDController.h"

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

void moveForward() {

  int16_t targetAngle, power, error, speedDifference, leftSpeed, rightSpeed;
  int16_t speed = 100;
  int16_t proportion = 12;
  float integral = 0.5;
  unsigned long duration = 750;
  unsigned long time;
  bool lineDetected = false;
  bool timedOut = false;
  String response;
  bool reverseNeeded = false;
  int numberOfLoops = 0;

  turnSensorReset();
  turnSensorUpdate();

  targetAngle = getHeading();

  time = millis();

  while(!timedOut) {

    turnSensorUpdate();

    error = wrapAngle(getHeading() - targetAngle);

    speedDifference = (error * proportion) + ((int)(error * integral));

    leftSpeed = speed + speedDifference;
    rightSpeed = speed - speedDifference;

    leftSpeed = constrain(leftSpeed, 0, (int16_t)speed);
    rightSpeed = constrain(rightSpeed, 0, (int16_t)speed);

    timedOut = ((millis() - time) > duration);

    if (timedOut) {
      leftSpeed = 0;
      rightSpeed = 0;
      lineDetected = false;
    }

    lineSensors.readCalibrated(lineSensorValues);
    lineDetected = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

    if (lineDetected) {
      leftSpeed = 0;
      rightSpeed = 0;
      timedOut = true;
      reverseNeeded = true;
    }
    motors.setSpeeds(leftSpeed,rightSpeed);
  }

  if (reverseNeeded) {
    reverseCorrection();
  }
}

void reverseCorrection() {
  bool finished = false;
  int leftSpeed = -50;
  int rightSpeed = -50;

  while (!finished) {
    lineSensors.readCalibrated(lineSensorValues);

    if ((lineSensorValues[0] < 200) && (lineSensorValues[4] < 200)) {
      leftSpeed = 0;
      rightSpeed = 0;
      finished = true;
    } else if ((lineSensorValues[0] >= 200) && (lineSensorValues[4] < 200)) {
      leftSpeed = -80;
      rightSpeed = 0;
    } else if ((lineSensorValues[0] < 200) && (lineSensorValues[4] >= 200)) {
      leftSpeed = 0;
      rightSpeed = -80;
    } else {
      leftSpeed = -50;
      rightSpeed = -50;
    }
    motors.setSpeeds(leftSpeed, rightSpeed);
  }

  delay(150);
  motors.setSpeeds(-50,-50);
  delay(100);
  motors.setSpeeds(0,0);
}

void moveForwardLineLeft() {
  int16_t leftSpeed = 75;
  int16_t rightSpeed = 75;
  bool finished = false;
  bool halfWall = false;
  bool correctFinished = false;
  int16_t beginTime;
  int16_t endTime;
  bool isWall = false;

  beginTime = millis();

  while (!finished) {
    
    lineSensors.readCalibrated(lineSensorValues);
    finished = ((lineSensorValues[0] >= 200) || (lineSensorValues[4] >= 200));

    if (finished) {
      leftSpeed = 0;
      rightSpeed = 0;
    }

    motors.setSpeeds(leftSpeed, rightSpeed);
  }

  motors.setSpeeds(100,100);
  delay(100);
  motors.setSpeeds(0,0);
  lineSensors.readCalibrated(lineSensorValues);
  printReadingsToSerial();

  if ((lineSensorValues[0] >= 200) && (lineSensorValues[4] <= 50)) {
      motors.setSpeeds(-100,-100);
      delay(200);
      rotateAngle(-90, 150);
      bool timedOut = false;
      bool breakLoop = false;
      motors.setSpeeds(100,100);
      int time = millis();
      int timeElapsed = 0;
      while (!breakLoop) {
        lineSensors.readCalibrated(lineSensorValues);
        isWall = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

        timeElapsed = millis();
        timedOut = ((timeElapsed - time) >= 2000);

        if (timedOut) {
          motors.setSpeeds(0,0);
          delay(200);
          motors.setSpeeds(-100,-100);
          delay(2000);
          breakLoop = true;
        }

        if(isWall) {
          motors.setSpeeds(0,0);
          delay(200);
          motors.setSpeeds(-100,-100);
          delay(200);
          breakLoop = true;
        }

      }
      motors.setSpeeds(0,0);
      rotateAngle(90,150);

    }

    if (isWall) {
      motors.setSpeeds(100,100);
      delay(1500);
      motors.setSpeeds(0,0);
      rotateAngle(90, 150);
    }

    lineSensors.readCalibrated(lineSensorValues);
    correctFinished = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

    delay(100);

    if(!correctFinished) {
      forwardCorrection();
    }

    delay(50);
}

void moveForwardLineRight() {
  int16_t leftSpeed = 75;
  int16_t rightSpeed = 75;
  bool finished = false;
  bool halfWall = false;
  bool correctFinished = false;
  int16_t beginTime;
  int16_t endTime;
  bool isWall = false;

  beginTime = millis();

  while (!finished) {
    
    lineSensors.readCalibrated(lineSensorValues);
    finished = ((lineSensorValues[0] >= 200) || (lineSensorValues[4] >= 200));

    if (finished) {
      leftSpeed = 0;
      rightSpeed = 0;
    }

    motors.setSpeeds(leftSpeed, rightSpeed);
  }

  motors.setSpeeds(100,100);
  delay(100);
  motors.setSpeeds(0,0);
  lineSensors.readCalibrated(lineSensorValues);
  printReadingsToSerial();

  if ((lineSensorValues[0] <= 50) && (lineSensorValues[4] >= 200)) {
      motors.setSpeeds(-100,-100);
      delay(200);
      rotateAngle(90, 150);
      bool timedOut = false;
      bool breakLoop = false;
      motors.setSpeeds(100,100);
      int time = millis();
      int timeElapsed = 0;
      while (!breakLoop) {
        lineSensors.readCalibrated(lineSensorValues);
        isWall = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

        timeElapsed = millis();
        timedOut = ((timeElapsed - time) >= 2000);

        if (timedOut) {
          motors.setSpeeds(0,0);
          delay(200);
          motors.setSpeeds(-100,-100);
          delay(2000);
          breakLoop = true;
        }

        if(isWall) {
          motors.setSpeeds(0,0);
          delay(200);
          motors.setSpeeds(-100,-100);
          delay(200);
          breakLoop = true;
        }

      }
      motors.setSpeeds(0,0);
      rotateAngle(-90,150);

    }

    if (isWall) {
      motors.setSpeeds(100,100);
      delay(1500);
      motors.setSpeeds(0,0);
      rotateAngle(-90, 150);
    }

    lineSensors.readCalibrated(lineSensorValues);
    correctFinished = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

    delay(100);

    if(!correctFinished) {
      forwardCorrection();
    }

    delay(50);
}

void forwardCorrection() {
  bool finished = false;
  int16_t leftSpeed = 50;
  int16_t rightSpeed = 50;
  uint32_t correctTime = 0;
  uint16_t differenceTime = 0;

  correctTime = millis();

  while (!finished) {
    lineSensors.readCalibrated(lineSensorValues);

    if ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200)) {
      differenceTime = millis() - correctTime;

      totalCorrectionTime += differenceTime;
      numOfCorrections++;
      avgCorrectionTime = totalCorrectionTime / numOfCorrections;
      leftSpeed = 0;
      rightSpeed = 0;
      finished = true;
    } else if ((lineSensorValues[0] >= 200) && (lineSensorValues[4] < 200)) {
      leftSpeed = 0;
      rightSpeed = 100;
    } else if ((lineSensorValues[0] < 200) && (lineSensorValues[4] >= 200)) {
      leftSpeed = 100;
      rightSpeed = 0;
    } else {
      leftSpeed = 70;
      rightSpeed = 70;
    }

    motors.setSpeeds(leftSpeed, rightSpeed);
    delay(50);
  }


}
