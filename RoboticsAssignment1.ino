#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ProximitySensors proxSensors;

uint16_t leftSpeed = 100;
uint16_t rightSpeed = 100;
uint16_t leftBackSpeed = -100;
uint16_t rightBackSpeed = -100;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (buttonA.isPressed()) {
    manualControl();
  } else if (buttonB.isPressed()) {
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

      if (incomingByte == 113) {
        running = false;
      }
    }
  }
}

void semiControl() {
  proxSensors.read();
  
}

void fullControl() {

}
