#include <Zumo32U4.h>

int16_t minBlackLine = 250;
int16_t maxBlackLine = 600;

int16_t leftSpeed = 100;
int16_t rightSpeed = 100;
int16_t leftBackSpeed = -100;
int16_t rightBackSpeed = -100;

//////////////////////////////////////////////////////////////////////////////////
/*
  Manual function. This function works by getting an incoming byte from
  Serial1 which will correspond to a specific action. This is then ran through
  lots of if statements to determine which action is needed.
*/
//////////////////////////////////////////////////////////////////////////////////

void manual() {

  bool running = true;

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

      Serial1.print("Found person.");
      ledRed(1);
      buzzer.playFrequency(500,1000,12);
      delay(200);
      ledRed(0);

    } else if (incomingByte == 49) {  //Increase speed to 100 (1) (Default Speed)
        
      Serial1.print("Speed set to 100.");
      leftSpeed = 100;
      rightSpeed = 100;
      leftBackSpeed = -100;
      rightBackSpeed = -100;   

    } else if (incomingByte == 50) {  //Increase speed to 200 (2)
        
      Serial1.print("Speed set to 200.");
      leftSpeed = 200;
      rightSpeed = 200;
      leftBackSpeed = -200;
      rightBackSpeed = -200;   
        
    } else if (incomingByte == 51) {  //Increase speed to 300 (3)
        
      Serial1.print("Speed set to 300.");
      leftSpeed = 300;
      rightSpeed = 300;
      leftBackSpeed = -300;
      rightBackSpeed = -300;   

    } else if (incomingByte == 52) {  //Increase speed to 400 (4) (Max Speed)
        
      Serial1.print("Speed set to 400.");
      leftSpeed = 400;
      rightSpeed = 400;
      leftBackSpeed = -400;
      rightBackSpeed = -400;   

    } else if (incomingByte == 113) {  //Rotate -90 degrees (Left) (q)

      Serial1.print("Rotating 90 degrees left.");
      rotateAngle(LEFT_TURN, TOP_SPEED);

    } else if (incomingByte == 101) {  //Rotate 90 degrees (Right) (e)

      Serial1.print("Rotating 90 degrees right.");
      rotateAngle(RIGHT_TURN, TOP_SPEED);

    } else if (incomingByte == 122) {  //Disconnects Zumo from GUI and exit manual mode (z)

      running = false;

    }

  }

  motors.setSpeeds(0,0);

}

//////////////////////////////////////////////////////////////////////////////////
/*
  Semi auto function. This function works by telling the Zumo to follow
  the left line all the way around the course. If the Zumo hits a right corner,
  it will return to manual mode which notifies the operator to make a right turn.
  Once this action has been performed, it will return back to following the left
  wall.
*/
//////////////////////////////////////////////////////////////////////////////////

void semiAuto() {

  bool running = true;
  bool exitMode = false;

  Serial1.print("Moving forwards.");

  while (running) {

    if (exitMode) {

      running = false;

    }

    incomingByte = Serial1.read();

    if (incomingByte == 122) {  //Exits mode and exits GUI (z)

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
      Serial1.print("Moving forwards.");

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

    proximityScan();

  }

  motors.setSpeeds(0,0);


}

//////////////////////////////////////////////////////////////////////////////////
/*
  Semi auto manual controls function. This function works by getting an incoming 
  byte from Serial1 which will correspond to a specific action. This is then ran 
  through some if statements to determine which action is needed. This is used in
  the semi auto function when it returns to manual mode when performing right turns.
*/
//////////////////////////////////////////////////////////////////////////////////

bool semiAutoManualControls(bool exitMode) {

  bool running = true;

  while (running) {

    incomingByte = Serial1.read();

    if (incomingByte == 32) {  //Finish. Notifies the Zumo that the rotations have completed (Spacebar)

      Serial1.print("Finished movements.");
      ledRed(1);
      delay(100);
      ledRed(0);
      running = false;

    } else if (incomingByte == 113) {  //Rotate -90 degrees (Left) (q)

      Serial1.print("Rotating 90 degrees left.");
      rotateAngle(LEFT_TURN, TOP_SPEED);

    } else if (incomingByte == 101) {  //Rotate 90 degrees (Right) (e)

      Serial1.print("Rotating 90 degrees right.");
      rotateAngle(RIGHT_TURN, TOP_SPEED);

    } else if (incomingByte == 122) {  //Exits mode and exits GUI (z)

      running = false;
      return exitMode = true;

    } else if (incomingByte == 119) { //Forward Control (w)
      
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

    } else if (incomingByte == 49) {  //Increase speed to 100 (1) (Default Speed)
        
      Serial1.print("Speed set to 100.");
      leftSpeed = 100;
      rightSpeed = 100;
      leftBackSpeed = -100;
      rightBackSpeed = -100;   

    } else if (incomingByte == 50) {  //Increase speed to 200 (2)
        
      Serial1.print("Speed set to 200.");
      leftSpeed = 200;
      rightSpeed = 200;
      leftBackSpeed = -200;
      rightBackSpeed = -200;   
        
    } else if (incomingByte == 51) {  //Increase speed to 300 (3)
        
      Serial1.print("Speed set to 300.");
      leftSpeed = 300;
      rightSpeed = 300;
      leftBackSpeed = -300;
      rightBackSpeed = -300;   

    } else if (incomingByte == 52) {  //Increase speed to 400 (4) (Max Speed)
        
      Serial1.print("Speed set to 400.");
      leftSpeed = 400;
      rightSpeed = 400;
      leftBackSpeed = -400;
      rightBackSpeed = -400;   

    }

  }

}

//////////////////////////////////////////////////////////////////////////////////
/*
  Full auto function. This function works by telling the Zumo to follow
  the left line all the way around the course. If the Zumo hits a right corner,
  it will automatically make a 90 degree turn right where it will then continue
  to go around the course.
*/
//////////////////////////////////////////////////////////////////////////////////

void fullAuto() {

  bool running = true;

  Serial1.print("Moving forwards.");

  while (running) {

    incomingByte = Serial1.read();

    if (incomingByte == 122) {  //Disconnects Zumo from GUI (z)

      running = false;

    }

    lineSensors.read(lineSensorValues);
    
    if (lineSensorValues[2] > maxBlackLine || lineSensorValues[4] > maxBlackLine) {  //If front and right sensors detect a black line, we have hit a right corner so make a 90 degree turn.
      
      Serial1.print("Hit corner, rotating.");
      motors.setSpeeds(-100,-100);
      delay(200);
      rotateAngle(RIGHT_TURN, TOP_SPEED);
      delay(200);
      Serial1.print("Moving forwards.");

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

    proximityScan();

  }

  motors.setSpeeds(0,0);

}

//////////////////////////////////////////////////////////////////////////////////
/*
  Proximity scan function. This function reads the proximity sensors on the Zumo
  and checks to see if any sensors reach the value of 6. If any reach 6, turn away
  from the object.
*/
//////////////////////////////////////////////////////////////////////////////////

void proximityScan() {

  proxSensors.read();

    if (((proxSensors.countsFrontWithLeftLeds()) >= 6) || (proxSensors.countsFrontWithRightLeds() >= 6)) {

      ledRed(1);
      //buzzer.playFrequency(500,1000,12);
      Serial1.print("Person detected in room!");
      delay(200);
      ledRed(0);
      rotateAngle(-45, TOP_SPEED);
      delay(200);
      motors.setSpeeds(100,100);
      delay(200);

    } else if ((proxSensors.countsLeftWithLeftLeds() >= 5) || (proxSensors.countsLeftWithRightLeds() >= 5)) {

      ledRed(1);
      //buzzer.playFrequency(500,1000,12);
      Serial1.print("Person detected in room!");
      delay(200);
      ledRed(0);
      rotateAngle(-45, TOP_SPEED);
      delay(200);
      motors.setSpeeds(100,100);
      delay(400);

    } else if ((proxSensors.countsRightWithLeftLeds() >= 5) || (proxSensors.countsRightWithRightLeds() >= 5)) {

      ledRed(1);
      //buzzer.playFrequency(500,1000,12);
      Serial1.print("Person detected in room!");
      delay(200);
      ledRed(0);
      rotateAngle(RIGHT_TURN, TOP_SPEED);
      delay(200);
      motors.setSpeeds(100,100);
      delay(400);
      
    }

}
