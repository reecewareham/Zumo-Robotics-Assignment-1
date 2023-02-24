#include <L3G.h>
#include <Zumo32U4.h>
#include "src/TurnSensor/TurnSensor.h"

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
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
    Serial1.print("test");
    //calibrateLineSensors();
    delay(2000);
    semiControl();
  }

}

void manualControl() {

  uint16_t incomingByte = 0;
  bool running = true;

  while (running) {

    if (Serial1.available() > 0) {

      incomingByte = Serial1.read();

      Serial1.print(incomingByte);

      if (incomingByte == 49) { //Increase speed to 100 (1) (Default Speed)
        
        leftSpeed = 100;
        rightSpeed = 100;
        leftBackSpeed = -100;
        rightBackSpeed = -100;    

      } else if (incomingByte == 50) { //Increase speed to 200 (2)
        
        leftSpeed = 200;
        rightSpeed = 200;
        leftBackSpeed = -200;
        rightBackSpeed = -200;
        
      } else if (incomingByte == 51) { //Increase speed to 300 (3)
        
        leftSpeed = 300;
        rightSpeed = 300;
        leftBackSpeed = -300;
        rightBackSpeed = -300;

      } else if (incomingByte == 52) { //Increase speed to 400 (4) (Max Speed)
        
        leftSpeed = 400;
        rightSpeed = 400;
        leftBackSpeed = -400;
        rightBackSpeed = -400;

      }

    
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

      }

      if (incomingByte == 122) {
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

    move();
    //running = false;

  }
}

void fullControl() {

}

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

int32_t getAngle() {
  return (((int32_t)turnAngle >> 16) *360) >> 16;
}

void move() {

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