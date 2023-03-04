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

uint16_t lineSensorValues[5] = { 0, 0, 0, 0, 0 };
uint16_t incomingByte = 0;

//////////////////////////////////////////////////////////////////////////////////
/*
  Setup. Creates a connection on Serial1. Enable the five line sensors on the Zumo
  and the two front proximity sensors on the Zumo.
*/
//////////////////////////////////////////////////////////////////////////////////

void setup() {

  Serial1.begin(9600);
  lineSensors.initFiveSensors();
  proxSensors.initFrontSensor();

}

//////////////////////////////////////////////////////////////////////////////////
/*
  Loop. Checks to see if Serial1 is available. If it is available, it will read
  incoming bytes. It will then check to see if the byte corresponds to the three
  actions that are available. If they match, it will run that statement. This
  decides which mode the Zumo will be put into.
*/
//////////////////////////////////////////////////////////////////////////////////

void loop() {

  if (Serial1.available() > 0) {

    incomingByte = Serial1.read();
  
    if ((incomingByte == 106) || (buttonA.isPressed())) {
      manualControl();
    } else if ((incomingByte == 107) || (buttonB.isPressed())) {
      delay(500);
      calibrateLineSensors();
      delay(2000);
      semiControl();
    } else if ((incomingByte == 108) || (buttonC.isPressed())) {
      delay(500);
      calibrateLineSensors();
      delay(2000);
      fullControl();
    }
  }

}

//////////////////////////////////////////////////////////////////////////////////
/*
  Manual control function. This function works by setting up the gyro and calling
  the manual function which will put the Zumo into manual mode.
*/
//////////////////////////////////////////////////////////////////////////////////

void manualControl() {

  turnSensorSetup();
  delay(500);
  turnSensorReset();

  manual();

}

//////////////////////////////////////////////////////////////////////////////////
/*
  Semi auto control function. This function works by setting up the gyro and calling
  the semi auto function which will put the Zumo into semi auto mode.
*/
//////////////////////////////////////////////////////////////////////////////////

void semiControl() {

  turnSensorSetup();
  delay(500);
  turnSensorReset();

  semiAuto();

}

//////////////////////////////////////////////////////////////////////////////////
/*
  Full auto control function. This function works by setting up the gyro and calling
  the full auto function which will put the Zumo into full auto mode.
*/
//////////////////////////////////////////////////////////////////////////////////

void fullControl() {

  turnSensorSetup();
  delay(500);
  turnSensorReset();
 
  fullAuto();

}
