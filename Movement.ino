#include <Zumo32U4.h>
#include "src/TurnSensor/TurnSensor.h"
#include "src/PIDController/PIDController.h"

#define ROT_PROPOTION 6.5
#define ROT_INTEGRAL 0.001
#define ROT_DIRIVATIVE 0.0

#define TRIG 2
#define ECHO 6
#define MAX_DISTANCE 30

void moveForward() {

  int16_t targetAngle, power, error, speedDifference, leftSpeed, rightSpeed;
  int16_t speed = 100;
  int16_t proportion = 12;
  float integral = 0.5;
  unsigned long duration = 1000;
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

  Serial1.print("Moving through corridor.");

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
  bool correctFinished = false;
  int16_t beginTime;
  int16_t endTime;
  bool isWall = false;
  bool timedOut = false;

  beginTime = millis();

  Serial1.print("Checking left wall.");

  while (!finished) {
    
    lineSensors.readCalibrated(lineSensorValues);
    finished = ((lineSensorValues[0] >= 200) || (lineSensorValues[4] >= 200));

    //endTime = ((millis() - beginTime) > 2000);

    if (finished) {
      leftSpeed = 0;
      rightSpeed = 0;
    }

    motors.setSpeeds(leftSpeed, rightSpeed);
  }

  motors.setSpeeds(100,100);
  delay(100);
  motors.setSpeeds(0,0);
  delay(100);
  lineSensors.readCalibrated(lineSensorValues);

  if ((lineSensorValues[0] >= 200) && (lineSensorValues[4] <= 50)) {
      Serial1.print("Detected half wall.");  
      motors.setSpeeds(-100,-100);
      delay(400);
      rotateAngle(-90, 150);
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
      Serial1.print("Found left corner. Moving around corner.");
      motors.setSpeeds(100,100);
      delay(2000);
      motors.setSpeeds(0,0);
      rotateAngle(90, 150);
    }

    if (timedOut) {
      Serial1.print("No wall detected. Must be left room.");
      motors.setSpeeds(100,100);
      delay(1200);
      motors.setSpeeds(0,0);

      rotateAndScan();    

      motors.setSpeeds(-100,-100);
      delay(1200);
      motors.setSpeeds(0,0);
      rotateAngle(-90,150);
      delay(100);
      motors.setSpeeds(100,100);
      delay(3000);
      motors.setSpeeds(0,0);
      rotateAngle(90,150);
    }

    lineSensors.readCalibrated(lineSensorValues);
    correctFinished = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

    delay(100);

    if(!correctFinished) {
      forwardCorrection();
    }

  motors.setSpeeds(-100,-100);
  delay(300);
  motors.setSpeeds(0,0);
  delay(50);
}

void moveForwardLineRight() {
  int16_t leftSpeed = 75;
  int16_t rightSpeed = 75;
  bool finished = false;
  bool correctFinished = false;
  int16_t beginTime;
  int16_t endTime;
  bool isWall = false;
  bool timedOut = false;

  beginTime = millis();

  Serial1.print("Checking right wall.");

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
  delay(100);
  lineSensors.readCalibrated(lineSensorValues);

  if ((lineSensorValues[0] <= 50) && (lineSensorValues[4] >= 200)) {
      Serial1.print("Detected half wall.");    
      motors.setSpeeds(-100,-100);
      delay(400);
      rotateAngle(90, 150);
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
      Serial1.print("Found right corner. Moving around corner.");
      motors.setSpeeds(100,100);
      delay(2000);
      motors.setSpeeds(0,0);
      rotateAngle(-90, 150);
    }

    if (timedOut) {
      Serial1.print("No wall detected. Must be right room.");
      motors.setSpeeds(100,100);
      delay(1200);
      motors.setSpeeds(0,0);

      rotateAndScan();

      motors.setSpeeds(-100,-100);
      delay(1200);
      motors.setSpeeds(0,0);
      rotateAngle(90,150);
      delay(100);
      motors.setSpeeds(100,100);
      delay(3000);
      motors.setSpeeds(0,0);
      rotateAngle(-90,150);
    }

    lineSensors.readCalibrated(lineSensorValues);
    correctFinished = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

    delay(100);

    if(!correctFinished) {
      forwardCorrection();
    }

  motors.setSpeeds(-100,-100);
  delay(300);
  motors.setSpeeds(0,0);
  delay(50);
}

void moveForwardSemi() {

  int16_t targetAngle, power, error, speedDifference, leftSpeed, rightSpeed;
  int16_t speed = 100;
  int16_t proportion = 12;
  float integral = 0.5;
  unsigned long duration = 1000;
  unsigned long time;
  bool lineDetected = false;
  bool timedOut = false;
  bool reverseNeeded = false;

  turnSensorReset();
  turnSensorUpdate();

  targetAngle = getHeading();

  time = millis();

  Serial1.print("Moving through corridor.");

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
    Serial1.print("Hit end of corridor. Zumo needs rotating.");
    semiAutoManualControls();
  }
}

void moveForwardLineLeftSemi() {
  int16_t leftSpeed = 75;
  int16_t rightSpeed = 75;
  bool finished = false;
  bool correctFinished = false;
  int16_t beginTime;
  int16_t endTime;
  bool isWall = false;
  bool timedOut = false;

  beginTime = millis();

  Serial1.print("Checking left wall.");

  while (!finished) {
    
    lineSensors.readCalibrated(lineSensorValues);
    finished = ((lineSensorValues[0] >= 200) || (lineSensorValues[4] >= 200));

    //endTime = ((millis() - beginTime) > 2000);

    if (finished) {
      leftSpeed = 0;
      rightSpeed = 0;
    }

    motors.setSpeeds(leftSpeed, rightSpeed);
  }

  motors.setSpeeds(100,100);
  delay(100);
  motors.setSpeeds(0,0);
  delay(100);
  lineSensors.readCalibrated(lineSensorValues);

  if ((lineSensorValues[0] >= 200) && (lineSensorValues[4] <= 50)) {
      Serial1.print("Detected half wall.");  
      motors.setSpeeds(-100,-100);
      delay(400);
      rotateAngle(-90, 150);
      bool breakLoop = false;
      motors.setSpeeds(100,100);
      int32_t time = millis();
      int32_t timeElapsed = 0;
      while (!breakLoop) {
        lineSensors.readCalibrated(lineSensorValues);
        isWall = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

        timeElapsed = millis();
        timedOut = ((timeElapsed - time) >= 2000);

        if (timedOut) {
          motors.setSpeeds(0,0);
          delay(200);
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
    }

    if (isWall) {
      Serial1.print("Found left corner. Zumo needs rotating.");
      semiAutoManualControls();
      motors.setSpeeds(100,100);
      delay(1500);
      motors.setSpeeds(0,0);
      rotateAngle(90, 150);
    }

    if (timedOut) {
      Serial1.print("No wall detected. Must be left room. Zumo needs rotating.");
      semiAutoManualControls();
      motors.setSpeeds(100,100);
      delay(1500);
      motors.setSpeeds(0,0);

      rotateAndScan();    

      motors.setSpeeds(-100,-100);
      delay(1500);
      motors.setSpeeds(0,0);
      rotateAngle(-90,150);
      delay(100);
      motors.setSpeeds(100,100);
      delay(1500);
      motors.setSpeeds(0,0);
      rotateAngle(90,150);
    }

    lineSensors.readCalibrated(lineSensorValues);
    correctFinished = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

    delay(100);

    if(!correctFinished) {
      forwardCorrection();
    }

  motors.setSpeeds(-100,-100);
  delay(300);
  motors.setSpeeds(0,0);
  delay(50);
}

void moveForwardLineRightSemi() {
  int16_t leftSpeed = 75;
  int16_t rightSpeed = 75;
  bool finished = false;
  bool correctFinished = false;
  int16_t beginTime;
  int16_t endTime;
  bool isWall = false;
  bool timedOut = false;

  beginTime = millis();

  Serial1.print("Checking right wall.");

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
  delay(100);
  lineSensors.readCalibrated(lineSensorValues);

  if ((lineSensorValues[0] <= 50) && (lineSensorValues[4] >= 200)) {
      Serial1.print("Detected half wall.");    
      motors.setSpeeds(-100,-100);
      delay(400);
      rotateAngle(90, 150);
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
    }

    if (isWall) {
      Serial1.print("Found right corner. Zumo needs rotating.");
      semiAutoManualControls();
      motors.setSpeeds(100,100);
      delay(1500);
      motors.setSpeeds(0,0);
      rotateAngle(-90, 150);
    }

    if (timedOut) {
      Serial1.print("No wall detected. Must be right room. Zumo needs rotating.");
      semiAutoManualControls();
      motors.setSpeeds(100,100);
      delay(1500);
      motors.setSpeeds(0,0);

      rotateAndScan();

      motors.setSpeeds(-100,-100);
      delay(1500);
      motors.setSpeeds(0,0);
      rotateAngle(90,150);
      delay(100);
      motors.setSpeeds(100,100);
      delay(100);
      motors.setSpeeds(0,0);
    }

    lineSensors.readCalibrated(lineSensorValues);
    correctFinished = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

    delay(100);

    if(!correctFinished) {
      forwardCorrection();
    }

  motors.setSpeeds(-100,-100);
  delay(300);
  motors.setSpeeds(0,0);
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

void rotateAndScan() {
  Serial1.print("Scanning room for objects.");
  bool scanRight = false;
  bool scanLeft = false;
  bool objectDetected = false;
  bool objectAlreadyFound = false;

  rotateAngle(90,150);
  delay(500);

  for (int i = 0; i < 4; i++) {
    proxSensors.read();
    if (((proxSensors.countsFrontWithLeftLeds()) >= 6) || (proxSensors.countsFrontWithRightLeds() >= 6)) {
      ledRed(1);
      delay(1000);
      ledRed(0);
      objectDetected = true;     
    }

    if (!objectAlreadyFound) {

      if (objectDetected) {
        motors.setSpeeds(100,100);
        delay(400);
        motors.setSpeeds(0,0);
        delay(100);
        ledRed(1);
        buzzer.playFrequency(500,1000,12);
        Serial1.print("Object detected in room!");
        delay(100);
        ledRed(0);
        delay(100);
        motors.setSpeeds(-100,-100);
        delay(400);
        motors.setSpeeds(0,0);
        objectAlreadyFound = true;
      }
    }

    objectDetected = false;

    delay(200);
    rotateAngle(-45,150);
    delay(200);
  }

  for (int i = 0; i < 4; i++) {
    proxSensors.read();
    if (((proxSensors.countsFrontWithLeftLeds()) >= 6) || (proxSensors.countsFrontWithRightLeds() >= 6)) {
      ledRed(1);
      delay(1000);
      ledRed(0);
      objectDetected = true;     
    }

    if (!objectAlreadyFound) {

      if (objectDetected) {
        motors.setSpeeds(100,100);
        delay(400);
        motors.setSpeeds(0,0);
        delay(100);
        ledRed(1);
        buzzer.playFrequency(500,1000,12);
        Serial1.print("Object detected in room!");
        delay(100);
        ledRed(0);
        delay(100);
        motors.setSpeeds(-100,-100);
        delay(400);
        motors.setSpeeds(0,0);
        objectAlreadyFound = true;
      }
    }

    objectDetected = false;

    delay(200);
    rotateAngle(45,150);
    delay(200);
  }

  rotateAngle(-90,150);
  delay(200);
}

void semiAutoManualControls() {

  uint16_t incomingByte = 0;
  bool running = true;

  turnSensorSetup();
  delay(500);
  turnSensorReset();

  while (running) {

    if (Serial1.available() > 0) {

      incomingByte = Serial1.read();

      Serial1.print(incomingByte);
    
      if (incomingByte == 119) { //Forward Control (w)
      
        ledYellow(1);
        motors.setSpeeds(leftSpeed, rightSpeed);
        delay(100);
        motors.setSpeeds(0, 0);
        ledYellow(0);

      } else if (incomingByte == 115) {  //Backwards Control (s)

        ledYellow(1);
        motors.setSpeeds(leftBackSpeed, rightBackSpeed);
        delay(100);
        motors.setSpeeds(0, 0);
        ledYellow(0);

      } else if (incomingByte == 97) {  //Left Control (a)

        ledYellow(1);
        motors.setSpeeds(leftBackSpeed, rightSpeed);
        delay(100);
        motors.setSpeeds(0, 0);
        ledYellow(0);

      } else if (incomingByte == 100) {  //Right Control (d)

        ledYellow(1);
        motors.setSpeeds(leftSpeed, rightBackSpeed);
        delay(100);
        motors.setSpeeds(0, 0);
        ledYellow(0);

      } else if (incomingByte == 32) {  //Finish (Spacebar). Notifies the zumo that the rotations have completed

        ledRed(1);
        delay(100);
        ledRed(0);
        running = false;

      } else if (incomingByte == 113) {  //Rotate -90 degrees (Left) (q)

        rotateAngle(90, 150);

      } else if (incomingByte == 101) {  //Rotate 90 degrees (Right) (e)

        rotateAngle(-90, 150);

      } else if (incomingByte == 122) {

        running = false;

      }
    }
  }
}
