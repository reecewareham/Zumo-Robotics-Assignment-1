import g4p_controls.*;
import processing.serial.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

GButton ForwardButton; 
GButton BackwardButton; 
GButton LeftButton; 
GButton RightButton; 
GButton DecreaseSpeedButton; 
GButton IncreaseSpeedButton; 
GButton NotifyButton; 
GTextArea MessageReceiveBox; 
GButton Rotate90Button;
GButton RotateLeft90Button;
GButton DisconnectButton; 
GButton DisconnectButtonSemi;
GButton ExitButton;
GWindow ManualControlWindow = null;
GWindow SemiAutoControlWindow = null;
GWindow FullAutoControlWindow = null;

Serial myPort;
String portName = "COM4";
int speed = 1;
String message;

public void setup(){
  
  myPort = new Serial(this, portName, 9600);
  
  size(600, 600, JAVA2D);
  createGUI();
  customGUI();
  // Place your setup code here
  
}

public void draw(){
  background(230);
  
  message = myPort.readString();
  if (message != null) {
    MessageReceiveBox.appendText(message + "\n");
  }
  myPort.clear();
  delay(500);
  
}

// Use this method to add additional statements
// to customise the GUI controls
public void customGUI(){

}



void createManualControlWindow() {
  ManualControlWindow = GWindow.getWindow(this, "Zumo Manual Control", 0, 0, 800, 800, JAVA2D);
  ManualControlWindow.noLoop();
  ManualControlWindow.setActionOnClose(G4P.CLOSE_WINDOW);
  ForwardButton = new GButton(ManualControlWindow, 350, 50, 100, 100);
  ForwardButton.setText("Forward");
  ForwardButton.addEventHandler(this, "ForwardButtonClick");
  BackwardButton = new GButton(ManualControlWindow, 350, 200, 100, 100);
  BackwardButton.setText("Backward");
  BackwardButton.addEventHandler(this, "BackwardButtonClick");
  LeftButton = new GButton(ManualControlWindow, 150, 200, 100, 100);
  LeftButton.setText("Left");
  LeftButton.addEventHandler(this, "LeftButtonClick");
  RightButton = new GButton(ManualControlWindow, 550, 200, 100, 100);
  RightButton.setText("Right");
  RightButton.addEventHandler(this, "RightButtonClick");
  DecreaseSpeedButton = new GButton(ManualControlWindow, 50, 350, 100, 100);
  DecreaseSpeedButton.setText("Decrease Speed (-)");
  DecreaseSpeedButton.setLocalColorScheme(GCScheme.RED_SCHEME);
  DecreaseSpeedButton.addEventHandler(this, "DecreaseSpeedButtonClick");
  IncreaseSpeedButton = new GButton(ManualControlWindow, 650, 350, 100, 100);
  IncreaseSpeedButton.setText("Increase Speed (+)");
  IncreaseSpeedButton.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  IncreaseSpeedButton.addEventHandler(this, "IncreaseSpeedButtonClick");
  NotifyButton = new GButton(ManualControlWindow, 250, 350, 300, 100);
  NotifyButton.setText("Notify");
  NotifyButton.setLocalColorScheme(GCScheme.ORANGE_SCHEME);
  NotifyButton.addEventHandler(this, "NotifyButtonClick");
  RotateLeft90Button = new GButton(ManualControlWindow, 50, 50, 100, 100);
  RotateLeft90Button.setText("Rotate Left  (-90°)");
  RotateLeft90Button.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  RotateLeft90Button.addEventHandler(this, "RotateLeft90ButtonClick");
  Rotate90Button = new GButton(ManualControlWindow, 650, 50, 100, 100);
  Rotate90Button.setText("Rotate Right (90°)");
  Rotate90Button.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  Rotate90Button.addEventHandler(this, "Rotate90ButtonClick");
  MessageReceiveBox = new GTextArea(ManualControlWindow, 500, 475, 275, 300, G4P.SCROLLBARS_VERTICAL_ONLY);
  MessageReceiveBox.setText("Messages From Zumo");
  MessageReceiveBox.setPromptText("Messages From Zumo");
  MessageReceiveBox.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  MessageReceiveBox.setOpaque(true);
  MessageReceiveBox.addEventHandler(this, "MessageReceiveBoxChange");
  DisconnectButton = new GButton(ManualControlWindow, 125, 675, 150, 75);
  DisconnectButton.setText("Disconnect");
  DisconnectButton.setLocalColorScheme(GCScheme.RED_SCHEME);
  DisconnectButton.addEventHandler(this, "DisconnectButtonClick");
  ManualControlWindow.loop();
}

void createSemiAutoControlWindow() {
  SemiAutoControlWindow = GWindow.getWindow(this, "Zumo Semi Auto Control", 0, 0, 800, 800, JAVA2D);
  SemiAutoControlWindow.noLoop();
  SemiAutoControlWindow.setActionOnClose(G4P.CLOSE_WINDOW);
  ForwardButton = new GButton(SemiAutoControlWindow, 350, 50, 100, 100);
  ForwardButton.setText("Forward");
  ForwardButton.addEventHandler(this, "ForwardButtonClick");
  BackwardButton = new GButton(SemiAutoControlWindow, 350, 200, 100, 100);
  BackwardButton.setText("Backward");
  BackwardButton.addEventHandler(this, "BackwardButtonClick");
  LeftButton = new GButton(SemiAutoControlWindow, 150, 200, 100, 100);
  LeftButton.setText("Left");
  LeftButton.addEventHandler(this, "LeftButtonClick");
  RightButton = new GButton(SemiAutoControlWindow, 550, 200, 100, 100);
  RightButton.setText("Right");
  RightButton.addEventHandler(this, "RightButtonClick");
  DecreaseSpeedButton = new GButton(SemiAutoControlWindow, 50, 350, 100, 100);
  DecreaseSpeedButton.setText("Decrease Speed (-)");
  DecreaseSpeedButton.setLocalColorScheme(GCScheme.RED_SCHEME);
  DecreaseSpeedButton.addEventHandler(this, "DecreaseSpeedButtonClick");
  IncreaseSpeedButton = new GButton(SemiAutoControlWindow, 650, 350, 100, 100);
  IncreaseSpeedButton.setText("Increase Speed (+)");
  IncreaseSpeedButton.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  IncreaseSpeedButton.addEventHandler(this, "IncreaseSpeedButtonClick");
  NotifyButton = new GButton(SemiAutoControlWindow, 250, 350, 300, 100);
  NotifyButton.setText("Notify");
  NotifyButton.setLocalColorScheme(GCScheme.ORANGE_SCHEME);
  NotifyButton.addEventHandler(this, "NotifyButtonClick");
  RotateLeft90Button = new GButton(SemiAutoControlWindow, 50, 50, 100, 100);
  RotateLeft90Button.setText("Rotate Left (-90°)");
  RotateLeft90Button.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  RotateLeft90Button.addEventHandler(this, "RotateLeft90ButtonClick");
  Rotate90Button = new GButton(SemiAutoControlWindow, 650, 50, 100, 100);
  Rotate90Button.setText("Rotate Right (90°)");
  Rotate90Button.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  Rotate90Button.addEventHandler(this, "Rotate90ButtonClick");
  MessageReceiveBox = new GTextArea(SemiAutoControlWindow, 500, 475, 275, 300, G4P.SCROLLBARS_VERTICAL_ONLY);
  MessageReceiveBox.setText("Messages From Zumo");
  MessageReceiveBox.setPromptText("Messages From Zumo");
  MessageReceiveBox.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  MessageReceiveBox.setOpaque(true);
  MessageReceiveBox.addEventHandler(this, "MessageReceiveBoxChange");
  DisconnectButtonSemi = new GButton(SemiAutoControlWindow, 125, 675, 150, 75);
  DisconnectButtonSemi.setText("Disconnect");
  DisconnectButtonSemi.setLocalColorScheme(GCScheme.RED_SCHEME);
  DisconnectButtonSemi.addEventHandler(this, "DisconnectButtonSemiClick");
  SemiAutoControlWindow.loop();
}

void createFullAutoControlWindow() {
  FullAutoControlWindow = GWindow.getWindow(this, "Zumo Full Auto Control", 0, 0, 800, 800, JAVA2D);
  FullAutoControlWindow.noLoop();
  FullAutoControlWindow.setActionOnClose(G4P.CLOSE_WINDOW);
  MessageReceiveBox = new GTextArea(FullAutoControlWindow, 200, 175, 400, 550, G4P.SCROLLBARS_VERTICAL_ONLY);
  MessageReceiveBox.setText("Messages From Zumo");
  MessageReceiveBox.setPromptText("Messages From Zumo");
  MessageReceiveBox.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  MessageReceiveBox.setOpaque(true);
  MessageReceiveBox.addEventHandler(this, "MessageReceiveBoxChange");
  ExitButton = new GButton(FullAutoControlWindow, 325, 50, 150, 75);
  ExitButton.setText("Exit");
  ExitButton.setLocalColorScheme(GCScheme.RED_SCHEME);
  ExitButton.addEventHandler(this, "ExitButtonClick");
  FullAutoControlWindow.loop();
}

public void ForwardButtonClick(GButton source, GEvent event) { //_CODE_:ForwardButton:779219:
  println("ForwardButton - GButton >> GEvent." + event + " @ " + millis());
  
  myPort.write("w");

} //_CODE_:ForwardButton:779219:

public void BackwardButtonClick(GButton source, GEvent event) { //_CODE_:BackwardButton:463829:
  println("BackwardButton - GButton >> GEvent." + event + " @ " + millis());

  myPort.write("s");

} //_CODE_:BackwardButton:463829:

public void RightButtonClick(GButton source, GEvent event) { //_CODE_:RightButton:403767:
  println("RightButton - GButton >> GEvent." + event + " @ " + millis());

  myPort.write("d");

} //_CODE_:RightButton:403767:

public void LeftButtonClick(GButton source, GEvent event) { //_CODE_:LeftButton:971842:
  println("LeftButton - GButton >> GEvent." + event + " @ " + millis());

  myPort.write("a");

} //_CODE_:LeftButton:971842:

public void DecreaseSpeedButtonClick(GButton source, GEvent event) { //_CODE_:DecreaseSpeedButton:404115:
  println("DecreaseSpeedButton - GButton >> GEvent." + event + " @ " + millis());
  
  speed--;
  if (speed < 1) {
   speed = 1;
  }
  
  if (speed == 1) {
   myPort.write("1"); 
  } else if (speed == 2) {
   myPort.write("2"); 
  } else if (speed == 3) {
   myPort.write("3"); 
  } else if (speed == 4) {
   myPort.write("4"); 
  }

} //_CODE_:DecreaseSpeedButton:404115:

public void IncreaseSpeedButtonClick(GButton source, GEvent event) { //_CODE_:IncreaseSpeedButton:327655:
  println("IncreaseSpeedButton - GButton >> GEvent." + event + " @ " + millis());
  
  speed++;
  if (speed > 4) {
   speed = 4;
  }
  
  if (speed == 1) {
   myPort.write("1"); 
  } else if (speed == 2) {
   myPort.write("2"); 
  } else if (speed == 3) {
   myPort.write("3"); 
  } else if (speed == 4) {
   myPort.write("4"); 
  }

} //_CODE_:IncreaseSpeedButton:327655:

public void NotifyButtonClick(GButton source, GEvent event) { //_CODE_:NotifyButton:586021:
  println("NotifyButton - GButton >> GEvent." + event + " @ " + millis());
  
  myPort.write(" ");
  
} //_CODE_:NotifyButton:586021:

public void MessageReceiveBoxChange(GTextArea source, GEvent event) { //_CODE_:MessageReceiveBox:448185:
  println("MessageReceiveBox - GTextArea >> GEvent." + event + " @ " + millis());
  
} //_CODE_:MessageReceiveBox:448185:

public void DisconnectButtonClick(GButton source, GEvent event) { //_CODE_:ExitButton:591591:
  println("DisconnectButton - GButton >> GEvent." + event + " @ " + millis());
  
  myPort.write("z");
  
  if (ManualControlWindow == null) {
    createManualControlWindow();
  } else {
    ManualControlWindow.setVisible(!ManualControlWindow.isVisible());
  }
  
} //_CODE_:ExitButton:591591:

public void DisconnectButtonSemiClick(GButton source, GEvent event) { //_CODE_:ExitButton:591591:
  println("DisconnectButtonSemi - GButton >> GEvent." + event + " @ " + millis());
  
  myPort.write("z");
  
  if (SemiAutoControlWindow == null) {
    createSemiAutoControlWindow();
  } else {
    SemiAutoControlWindow.setVisible(!SemiAutoControlWindow.isVisible());
  }
  
} //_CODE_:ExitButton:591591:

public void ExitButtonClick(GButton source, GEvent event) { //_CODE_:button1:727712:
  println("ExitButton - GButton >> GEvent." + event + " @ " + millis());
  
  myPort.write("z");
  
  if (FullAutoControlWindow == null) {
    createFullAutoControlWindow();
  } else {
    FullAutoControlWindow.setVisible(!FullAutoControlWindow.isVisible());
  }
  
} //_CODE_:button1:727712:

public void RotateLeft90ButtonClick(GButton source, GEvent event) { //_CODE_:button1:727712:
  println("RotateLeft90Button - GButton >> GEvent." + event + " @ " + millis());
  
  myPort.write("q");
  
} //_CODE_:button1:727712:

public void Rotate90ButtonClick(GButton source, GEvent event) { //_CODE_:button1:727712:
  println("Rotate90Button - GButton >> GEvent." + event + " @ " + millis());
  
  myPort.write("e");
  
} //_CODE_:button1:727712:
