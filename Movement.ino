#include <Zumo32U4.h>
#include "src/TurnSensor/TurnSensor.h"
#include "src/PIDController/PIDController.h"

#define ROT_PROPOTION 6.5
#define ROT_INTEGRAL 0.001
#define ROT_DIRIVATIVE 0.0

#define TRIG 2
#define ECHO 6
#define MAX_DISTANCE 30

int16_t minBlackLine = 250;
int16_t maxBlackLine = 600;

/* Old Method Code

void moveForward() {

  int16_t targetAngle, power, error, speedDifference, leftSpeed, rightSpeed;
  int16_t speed = 75;
  int16_t proportion = 12;
  float integral = 0.5;
  unsigned long duration = 800;
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
    rotateAngle(-90,150);
    rotateAngle(-90,150);
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
  int16_t leftSpeed = 50;
  int16_t rightSpeed = 50;
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

  motors.setSpeeds(50,50);
  delay(100);
  motors.setSpeeds(0,0);
  delay(100);
  lineSensors.readCalibrated(lineSensorValues);

  if ((lineSensorValues[0] >= 200) && (lineSensorValues[4] <= 50)) {
      Serial1.print("Detected half wall.");  
      motors.setSpeeds(-50,-50);
      delay(400);
      rotateAngle(-90, 150);
      bool breakLoop = false;
      motors.setSpeeds(65,65);
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
          numberOfCountsLeft++;
        }

        if(isWall) {
          motors.setSpeeds(0,0);
          delay(200);
          motors.setSpeeds(-50,-50);
          delay(200);
          breakLoop = true;
          numberOfCountsLeft++;
        }

      }
      motors.setSpeeds(0,0);
      rotateAngle(90,150);

    }

    if (numberOfCountsLeft = 3) {
      rotateAngle(-90,150);
      motors.setSpeeds(75,75);
      delay(1000);
      motors.setSpeeds(0,0);

    } else {

    if (isWall) {
      Serial1.print("Found left corner. Moving around corner.");
      motors.setSpeeds(50,50);
      delay(2000);
      motors.setSpeeds(0,0);
      rotateAngle(90, 150);
    }

    if (timedOut) {
      Serial1.print("No wall detected. Must be left room.");
      motors.setSpeeds(65,65);
      delay(1600);
      motors.setSpeeds(0,0);

      rotateAndScan();    

      motors.setSpeeds(-65,-65);
      delay(1100);
      motors.setSpeeds(0,0);
      rotateAngle(-90,150);
      delay(100);
      motors.setSpeeds(75,75);
      delay(2500);
      motors.setSpeeds(0,0);
      rotateAngle(90,150);
    }
    }

    lineSensors.readCalibrated(lineSensorValues);
    correctFinished = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

    delay(100);

    if(!correctFinished) {
      forwardCorrection();
    }

  motors.setSpeeds(-50,-50);
  delay(300);
  motors.setSpeeds(0,0);
  delay(50);
}

void moveForwardLineRight() {
  int16_t leftSpeed = 50;
  int16_t rightSpeed = 50;
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

  motors.setSpeeds(50,50);
  delay(100);
  motors.setSpeeds(0,0);
  delay(100);
  lineSensors.readCalibrated(lineSensorValues);

  if ((lineSensorValues[0] <= 50) && (lineSensorValues[4] >= 200)) {
      Serial1.print("Detected half wall.");    
      motors.setSpeeds(-50,-50);
      delay(400);
      rotateAngle(90, 150);
      bool breakLoop = false;
      motors.setSpeeds(75,75);
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
          numberOfCountsRight++;
        }

        if(isWall) {
          motors.setSpeeds(0,0);
          delay(200);
          motors.setSpeeds(-50,-50);
          delay(200);
          breakLoop = true;
          numberOfCountsRight++;
        }

      }
      motors.setSpeeds(0,0);
      rotateAngle(-90,150);

    }

    if (numberOfCountsRight = 3) {
      rotateAngle(90,150);
      motors.setSpeeds(75,75);
      delay(1000);
      motors.setSpeeds(0,0);

    } else {

    if (isWall) {
      Serial1.print("Found right corner. Moving around corner.");
      motors.setSpeeds(50,50);
      delay(2000);
      motors.setSpeeds(0,0);
      rotateAngle(-90, 150);
    }

    if (timedOut) {
      Serial1.print("No wall detected. Must be right room.");
      motors.setSpeeds(50,50);
      delay(1500);
      motors.setSpeeds(0,0);

      rotateAndScan();

      motors.setSpeeds(-50,-50);
      delay(1200);
      motors.setSpeeds(0,0);
      rotateAngle(90,150);
      delay(100);
      motors.setSpeeds(75,75);
      delay(3000);
      motors.setSpeeds(0,0);
      rotateAngle(-90,150);
    }
    }

    lineSensors.readCalibrated(lineSensorValues);
    correctFinished = ((lineSensorValues[0] >= 200) && (lineSensorValues[4] >= 200));

    delay(100);

    if(!correctFinished) {
      forwardCorrection();
    }

  motors.setSpeeds(-50,-50);
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
        motors.setSpeeds(50,50);
        delay(400);
        motors.setSpeeds(0,0);
        delay(100);
        ledRed(1);
        buzzer.playFrequency(500,1000,12);
        Serial1.print("Object detected in room!");
        delay(100);
        ledRed(0);
        delay(100);
        motors.setSpeeds(-50,-50);
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
        motors.setSpeeds(50,50);
        delay(400);
        motors.setSpeeds(0,0);
        delay(100);
        ledRed(1);
        buzzer.playFrequency(500,1000,12);
        Serial1.print("Object detected in room!");
        delay(100);
        ledRed(0);
        delay(100);
        motors.setSpeeds(-50,-50);
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

*/

void manual() {

  bool running = true;

  uint16_t leftSpeed = 100;
  uint16_t rightSpeed = 100;
  uint16_t leftBackSpeed = -100;
  uint16_t rightBackSpeed = -100;

  while (running) {

    incomingByte = Serial1.read();
    
    if (incomingByte == 119) { //Forward Control (w)
      
      Serial1.print("Moving forwards.");
      ledYellow(1);
      motors.setSpeeds(leftSpeed, rightSpeed);
      delay(200);
      motors.setSpeeds(0, 0);
      ledYellow(0);

    } else if (incomingByte == 115) {  //Backwards Control (s)

      Serial1.print("Moving backwards.");
      ledYellow(1);
      motors.setSpeeds(leftBackSpeed, rightBackSpeed);
      delay(200);
      motors.setSpeeds(0, 0);
      ledYellow(0);

    } else if (incomingByte == 97) {  //Left Control (a)

      Serial1.print("Turning left.");
      ledYellow(1);
      motors.setSpeeds(leftBackSpeed, rightSpeed);
      delay(200);
      motors.setSpeeds(0, 0);
      ledYellow(0);

    } else if (incomingByte == 100) {  //Right Control (d)

      Serial1.print("Turning right.");        
      ledYellow(1);
      motors.setSpeeds(leftSpeed, rightBackSpeed);
      delay(200);
      motors.setSpeeds(0, 0);
      ledYellow(0);

    } else if (incomingByte == 32) {  //Notify (Spacebar). Makes noise to notify that there is a object in a room

      Serial1.print("Found object.");
      ledRed(1);
      buzzer.playFrequency(500,1000,12);
      delay(200);
      ledRed(0);

    } else if (incomingByte == 49) {  //Increase speed to 100 (1) (Default Speed)
        
      Serial1.print("Speed set to 100.");
      leftSpeed, rightSpeed = 100;
      leftBackSpeed, rightBackSpeed = -100;  

    } else if (incomingByte == 50) {  //Increase speed to 200 (2)
        
      Serial1.print("Speed set to 200.");
      leftSpeed, rightSpeed = 200;
      leftBackSpeed, rightBackSpeed = -200; 
        
    } else if (incomingByte == 51) {  //Increase speed to 300 (3)
        
      Serial1.print("Speed set to 300.");
      leftSpeed, rightSpeed = 300;
      leftBackSpeed, rightBackSpeed = -300; 

    } else if (incomingByte == 52) {  //Increase speed to 400 (4) (Max Speed)
        
      Serial1.print("Speed set to 400.");
      leftSpeed, rightSpeed = 400;
      leftBackSpeed, rightBackSpeed = -400; 

    } else if (incomingByte == 113) {  //Rotate -90 degrees (Left) (q)

      Serial1.print("Rotating 90 degrees left.");
      rotateAngle(LEFT_TURN, TOP_SPEED);

    } else if (incomingByte == 101) {  //Rotate 90 degrees (Right) (e)

      Serial1.print("Rotating 90 degrees right.");
      rotateAngle(RIGHT_TURN, TOP_SPEED);

    } else if (incomingByte == 122) {  //Disconnects zumo from GUI and exit manual mode (z)

      running = false;

    }

  }

}

void semiAuto() {

  bool running = true;
  bool exitMode = false;

  while (running) {

    if (exitMode) {

      running = false;

    }

    lineSensors.read(lineSensorValues);
    
    if (lineSensorValues[2] > maxBlackLine || lineSensorValues[4] > maxBlackLine) {  //If front and right sensors detect a black line, we have hit a right corner so return to manual control so the operator can turn 90 degrees.
      motors.setSpeeds(-100,-100);
      delay(200);
      motors.setSpeeds(0,0);
      Serial1.print("Reached right corner, returning to manual control.");
      semiAutoManualControls(exitMode);
      delay(200);
    } else if (lineSensorValues[0] < maxBlackLine && lineSensorValues[0] > minBlackLine) {  //If left sensor is within the minimum black line and maximum black line, continuing going forward.
      motors.setSpeeds(100,100);
    } else if (lineSensorValues[0] > maxBlackLine) {  //If left sensor is greater than the maximum black line, then set right motor to reverse to align zumo back into the assigned range.
      motors.setSpeeds(100,-100);
    } else if (lineSensorValues[0] < minBlackLine) {  //If left sensor is smaller than the minimum black line, then set left motor to reverse to align zumo back into the assigned range.
      motors.setSpeeds(100,100);
      delay(25);
      motors.setSpeeds(-100,100);
      delay(75);
    } 

  }

}

bool semiAutoManualControls(bool exitMode) {

  bool running = true;

  while (running) {

    incomingByte = Serial1.read();

    if (incomingByte == 32) {  //Finish (Spacebar). Notifies the zumo that the rotations have completed

      Serial1.print("Finished movements.");
      ledRed(1);
      delay(100);
      ledRed(0);
      running = false;

    } else if (incomingByte == 113) {  //Rotate -90 degrees (Left) (q)

      Serial1.print("Rotating 90 degrees left.");
      rotateAngle(90, 150);

    } else if (incomingByte == 101) {  //Rotate 90 degrees (Right) (e)

      Serial1.print("Rotating 90 degrees right.");
      rotateAngle(-90, 150);

    } else if (incomingByte == 122) {  //Exits mode and exits GUI (z)

      running = false;
      return exitMode = true;

    }
  }
}

void fullAuto() {
  bool running = true;

  while (running) {
    /*motors.setSpeeds(75,75);
    lineSensors.readCalibrated(lineSensorValues);
    if (lineSensorValues[0] > 500) {
      motors.setSpeeds(75,50);
      delay(100);
      motors.setSpeeds(50,75);
    } else if (lineSensorValues[0] < 100) {
      motors.setSpeeds(50,75);
      delay(100);
      motors.setSpeeds(75,75);
    } else if (lineSensorValues[2] > 500) {
      motors.setSpeeds(0,0);
      delay(100);
      motors.setSpeeds(-75,-75);
      delay(100);
      motors.setSpeeds(0,0);
      delay(100);
      rotateAngle(-90,150);
      motors.setSpeeds(75,75);
    }
    delay(600);
    if (lineSensorValues[0] < 50) {
      motors.setSpeeds(0,0);
      delay(100);
      rotateAngle(90,150);
      delay(100);
      motors.setSpeeds(75,75);
    }
    if (((proxSensors.countsFrontWithLeftLeds()) >= 6) || (proxSensors.countsFrontWithRightLeds() >= 6)) {
      ledRed(1);
      buzzer.playFrequency(500,1000,12);
      Serial1.print("Object detected in room!");
      delay(200);
      ledRed(0);
    }*/

    incomingByte = Serial1.read();

    if (incomingByte == 122) {  //Disconnects zumo from GUI (z)

      running = false;

    }

    lineSensors.read(lineSensorValues);
    
    if (lineSensorValues[2] > maxBlackLine || lineSensorValues[4] > maxBlackLine) {  //If front and right sensors detect a black line, we have hit a right corner so make a 90 degree turn.
      motors.setSpeeds(-100,-100);
      delay(200);
      rotateAngle(-90,150);
      delay(200);
    } else if (lineSensorValues[0] < maxBlackLine && lineSensorValues[0] > minBlackLine) {  //If left sensor is within the minimum black line and maximum black line, continuing going forward.
      motors.setSpeeds(100,100);
    } else if (lineSensorValues[0] > maxBlackLine) {  //If left sensor is greater than the maximum black line, then set right motor to reverse to align zumo back into the assigned range.
      motors.setSpeeds(100,-100);
    } else if (lineSensorValues[0] < minBlackLine) {  //If left sensor is smaller than the minimum black line, then set left motor to reverse to align zumo back into the assigned range.
      motors.setSpeeds(100,100);
      delay(25);
      motors.setSpeeds(-100,100);
      delay(75);
    } 
  
  }

}
