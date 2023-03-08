# Robotics Assignment 1 - Search and Rescue

## Introduction

The goal of this assignment was to use a Zumo 32U4 to navigate a building and find any people that may be trapped in the building.
I was tasked with creating three different modes, manual, semi-auto and full auto. These three modes change the way the building is
navigated.

Manual Control - Navigates the building using manual controls. Requires an operator.

Semi Auto Control - Navigates the building automatically and finds people automatically but requires the operator to make turns.

Full Auto Control - Navigates the building automatically and finds people automatically with no human intervention.

## Installation

Installation is quick and easy as it only requires a few steps to get working.

### Zumo 

1) Download the ZumoRoboticAssignment directory containing the files from the repository.

2) Place the ZumoRoboticsAssignment directory into the sketchbook of the Arduino IDE.

3) Open the sketchbook in Arduino IDE and select ZumoRoboticsAssignment.

4) Plug the Zumo into the computer using a USB cable.

5) Select the Pololu Zumo 32U4 port from the list.

6) Click upload.

### GUI

1) Download the ZumoRoboticsAssignmentGUI directory containing the files from the repository.

2) Place the ZumoRoboticsAssignment directory into the sketchbook of the Processing IDE.

3) Open the sketchbook in Processing IDE and select ZumoRoboticsAssignmentGUI.

4) Plug XBee into computer using a USB cable.

5) Change the portName variable to the port the XBee is plugged into. E.g. "COM4".

6) Run the GUI.

## Using the GUI and Zumo

Using the GUI and Zumo is simple. 

Opening the GUI will show you three buttons which indicate the three different modes. Selecting
either semi auto or full auto modes will initiate a gyro setup and line sensor setup. Make sure the
Zumo is placed in the course or on the lines when selecting either option as it will need to calibrate
these sensors before starting. After both have completed, place the Zumo in the centre of the starting
area because after 5 seconds it will start moving.

If manual mode is selected, the GUI will open a window with some control buttons on the screen.

Forward - Moves forwards.

Backward - Moves backwards.

Left - Turns left.

Right - Turns right.

Decrease Speed - Decreases speed by 100.

Increase Speed - Increases speed by 100.

Rotate Left - Rotates left by 90 degrees.

Rotate Right - Rotates right by 90 degrees.

Notify - Makes a high pitched noise. Used to signal if there is a person.

Disconnect - Disables manual mode and returns to main GUI screen.

If semi auto mode is selected, the GUI will also open a window with some controls.

Forward - Moves forwards.

Backward - Moves backwards.

Left - Turns left.

Right - Turns right.

Decrease Speed - Decreases speed by 100.

Increase Speed - Increases speed by 100.

Rotate Left - Rotates left by 90 degrees.

Rotate Right - Rotates right by 90 degrees.

Finish - Notifies the Zumo that the manual control is finished and should return to semi auto.

Disconnect - Disables manual mode and returns to main GUI screen.






