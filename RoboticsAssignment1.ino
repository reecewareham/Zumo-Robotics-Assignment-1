#include <L3G.h>
#include <Zumo32U4.h>
#include "src/TurnSensor/TurnSensor.h"
#include <PololuBuzzer.h>

PololuBuzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
L3G gyro;
Zumo32U4LCD lcd;

#define LEFT_TURN 90.0
#define RIGHT_TURN -90.0
#define TOP_SPEED 150
#define TIME 500

uint16_t leftSpeed = 100;
uint16_t rightSpeed = 100;
uint16_t leftBackSpeed = -100;
uint16_t rightBackSpeed = -100;
uint16_t lineSensorValues[5] = { 0, 0, 0, 0, 0 };

uint32_t avgCorrectionTime = 0;
uint32_t totalCorrectionTime = 0;
uint16_t numOfCorrections = 0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  lineSensors.initFiveSensors();
  proxSensors.initFrontSensor();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (buttonA.isPressed()) {
    manualControl();
  } else if (buttonB.isPressed()) {
    delay(500);
    calibrateLineSensors();
    delay(2000);
    semiControl();
  } else if (buttonC.isPressed()) {
    delay(500);
    calibrateLineSensors();
    delay(2000);
    fullControl();
  }

}

void manualControl() {

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

      } else if (incomingByte == 32) {  //Notify (Spacebar). Makes noise to notify that there is a object in a room

        ledRed(1);
        buzzer.playFrequency(500,1000,12);
        delay(100);
        ledRed(0);

      } else if (incomingByte == 49) {  //Increase speed to 100 (1) (Default Speed)
        
        leftSpeed = 100;
        rightSpeed = 100;
        leftBackSpeed = -100;
        rightBackSpeed = -100;    

      } else if (incomingByte == 50) {  //Increase speed to 200 (2)
        
        leftSpeed = 200;
        rightSpeed = 200;
        leftBackSpeed = -200;
        rightBackSpeed = -200;
        
      } else if (incomingByte == 51) {  //Increase speed to 300 (3)
        
        leftSpeed = 300;
        rightSpeed = 300;
        leftBackSpeed = -300;
        rightBackSpeed = -300;

      } else if (incomingByte == 52) {  //Increase speed to 400 (4) (Max Speed)
        
        leftSpeed = 400;
        rightSpeed = 400;
        leftBackSpeed = -400;
        rightBackSpeed = -400;

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

void semiControl() {

  bool running = true;

  turnSensorSetup();
  delay(500);
  turnSensorReset();

  while (running) {

    moveSemiAuto();

  }
}

void fullControl() {

  bool running = true;

  turnSensorSetup();
  delay(500);
  turnSensorReset();

  while (running) {

    moveFullAuto();

  }

}

void moveFullAuto() {

  moveForward();
  delay(500);
  rotateAngle(LEFT_TURN, TOP_SPEED);
  delay(500);
  moveForwardLineLeft();
  delay(500);
  rotateAngle(RIGHT_TURN, TOP_SPEED);
  delay(500);
  rotateAngle(RIGHT_TURN, TOP_SPEED);
  delay(500);
  moveForwardLineRight();
  delay(500);
  rotateAngle(LEFT_TURN, TOP_SPEED);
  delay(500);
}

void moveSemiAuto() {

  moveForward();
  delay(500);
  rotateAngle(LEFT_TURN, TOP_SPEED);
  delay(500);
  moveForwardLineLeftSemi();
  delay(500);
  rotateAngle(RIGHT_TURN, TOP_SPEED);
  delay(500);
  rotateAngle(RIGHT_TURN, TOP_SPEED);
  delay(500);
  moveForwardLineRightSemi();
  delay(500);
  rotateAngle(LEFT_TURN, TOP_SPEED);
  delay(500);
}