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

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  lineSensors.initFiveSensors();
  proxSensors.initFrontSensor();
}

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

void manualControl() {

  turnSensorSetup();
  delay(500);
  turnSensorReset();

  manual();

}

void semiControl() {

  turnSensorSetup();
  delay(500);
  turnSensorReset();

  semiAuto();

}

void fullControl() {

  turnSensorSetup();
  delay(500);
  turnSensorReset();
 
  fullAuto();

}
