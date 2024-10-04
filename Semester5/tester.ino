#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize the LCD with the I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Define button pins
const int buttonUp = 2;
const int buttonDown = 3;
const int buttonSelect = 4;

// Servo motor
Servo myServo;
int servoPin = 9;
int pulseWidth = 500; // Initial pulse width in microseconds
int servoAngle = 0; // Initial angle

// ESC variables
Servo esc;
int escPin = 10; // PWM pin for ESC
int escPulseWidth = 1000; // Initial pulse width for ESC (min throttle)
int escRPM = 0; // Initial RPM

// Menu variables
int menuIndex = 0;
int submenuIndex = 0;
bool inSubmenu = false;
bool adjustingPulseWidth = false;
bool adjustingAngle = false;
bool adjustingEscPulseWidth = false;
const int menuItems = 3; // Increased to include "ESC"
const int submenuItems = 3; // Increased to include "Set Pulse Width"
String menu[menuItems] = {"Servo Control", "ESC", "Back"};
String submenu[submenuItems] = {"Set Angle", "Set Pulse Width", "Back"};
String escSubmenu[submenuItems] = {"Set Pulse Width", "Back"};

void setup() {
  // Initialize the LCD
  lcd.begin();
  lcd.backlight();

  // Initialize buttons
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonSelect, INPUT);

  // Initialize servo
  myServo.attach(servoPin);
  myServo.writeMicroseconds(pulseWidth);

  // Initialize ESC
  esc.attach(escPin);
  esc.writeMicroseconds(escPulseWidth);

  // Display the initial menu
  displayMenu();
}

void loop() {
  // Check if the up button is pressed
  if (digitalRead(buttonUp) == HIGH) {
    if (adjustingPulseWidth) {
      pulseWidth += 10;
      if (pulseWidth > 5000) {
        pulseWidth = 5000;
      }
      myServo.writeMicroseconds(pulseWidth);
      displayPulseWidthAndAngle();
    } else if (adjustingAngle) {
      servoAngle += 5;
      if (servoAngle > 180) {
        servoAngle = 180;
      }
      myServo.write(servoAngle);
      displayServoAngle();
    } else if (adjustingEscPulseWidth) {
      escPulseWidth += 10;
      if (escPulseWidth > 2000) { // Max throttle
        escPulseWidth = 2000;
      }
      esc.writeMicroseconds(escPulseWidth);
      displayEscPulseWidthAndRPM();
    } else if (inSubmenu) {
      submenuIndex--;
      if (submenuIndex < 0) {
        submenuIndex = submenuItems - 1;
      }
      if (menuIndex == 0) {
        displaySubmenu();
      } else if (menuIndex == 1) {
        displayEscSubmenu();
      }
    } else {
      menuIndex--;
      if (menuIndex < 0) {
        menuIndex = menuItems - 1;
      }
      displayMenu();
    }
    delay(200); // Simple debounce delay
  }

  // Check if the down button is pressed
  if (digitalRead(buttonDown) == HIGH) {
    if (adjustingPulseWidth) {
      pulseWidth -= 10;
      if (pulseWidth < 100) {
        pulseWidth = 100;
      }
      myServo.writeMicroseconds(pulseWidth);
      displayPulseWidthAndAngle();
    } else if (adjustingAngle) {
      servoAngle -= 5;
      if (servoAngle < 0) {
        servoAngle = 0;
      }
      myServo.write(servoAngle);
      displayServoAngle();
    } else if (adjustingEscPulseWidth) {
      escPulseWidth -= 10;
      if (escPulseWidth < 1000) { // Min throttle
        escPulseWidth = 1000;
      }
      esc.writeMicroseconds(escPulseWidth);
      displayEscPulseWidthAndRPM();
    } else if (inSubmenu) {
      submenuIndex++;
      if (submenuIndex >= submenuItems) {
        submenuIndex = 0;
      }
      if (menuIndex == 0) {
        displaySubmenu();
      } else if (menuIndex == 1) {
        displayEscSubmenu();
      }
    } else {
      menuIndex++;
      if (menuIndex >= menuItems) {
        menuIndex = 0;
      }
      displayMenu();
    }
    delay(200); // Simple debounce delay
  }

  // Check if the select button is pressed
  if (digitalRead(buttonSelect) == HIGH) {
    if (adjustingPulseWidth) {
      adjustingPulseWidth = false;
      inSubmenu = true;
      displaySubmenu();
    } else if (adjustingAngle) {
      adjustingAngle = false;
      inSubmenu = true;
      displaySubmenu();
    } else if (adjustingEscPulseWidth) {
      adjustingEscPulseWidth = false;
      inSubmenu = true;
      displayEscSubmenu();
    } else if (inSubmenu) {
      if (menuIndex == 0) {
        if (submenuIndex == 0) {
          // Set Angle
          adjustingAngle = true;
          displayServoAngle();
        } else if (submenuIndex == 1) {
          // Set Pulse Width
          adjustingPulseWidth = true;
          displayPulseWidthAndAngle();
        } else if (submenuIndex == 2) {
          // Back
          inSubmenu = false;
          displayMenu();
        }
      } else if (menuIndex == 1) {
        if (submenuIndex == 0) {
          // Set ESC Pulse Width
          adjustingEscPulseWidth = true;
          displayEscPulseWidthAndRPM();
        } else if (submenuIndex == 1) {
          // Back
          inSubmenu = false;
          displayMenu();
        }
      }
    } else {
      if (menuIndex == 0) {
        // Enter Servo Control submenu
        inSubmenu = true;
        submenuIndex = 0;
        displaySubmenu();
      } else if (menuIndex == 1) {
        // Enter ESC submenu
        inSubmenu = true;
        submenuIndex = 0;
        displayEscSubmenu();
      } else if (menuIndex == 2) {
        // Back from main menu
        // Add any additional logic if needed
      }
    }
    delay(200); // Simple debounce delay
  }
}

void displayMenu() {
  lcd.clear();
  for (int i = 0; i < menuItems; i++) {
    lcd.setCursor(0, i);
    if (i == menuIndex) {
      lcd.print("> ");
    } else {
      lcd.print("  ");
    }
    lcd.print(menu[i]);
  }
}

void displaySubmenu() {
  lcd.clear();
  for (int i = 0; i < submenuItems; i++) {
    lcd.setCursor(0, i);
    if (i == submenuIndex) {
      lcd.print("> ");
    } else {
      lcd.print("  ");
    }
    lcd.print(submenu[i]);
  }
}

void displayEscSubmenu() {
  lcd.clear();
  for (int i = 0; i < submenuItems; i++) {
    lcd.setCursor(0, i);
    if (i == submenuIndex) {
      lcd.print("> ");
    } else {
      lcd.print("  ");
    }
    lcd.print(escSubmenu[i]);
  }
}

void displayServoAngle() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angle: ");
  lcd.print(servoAngle);
}

void displayPulseWidthAndAngle() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pulse Width: ");
  lcd.print(pulseWidth);
  lcd.print(" us");

  // Calculate the corresponding angle
  int angle;
  if (pulseWidth < 500) {
    angle = map(pulseWidth, 100, 500, -90, 0);
  } else if (pulseWidth <= 2500) {
    angle = map(pulseWidth, 500, 2500, 0, 180);
  } else {
    angle = map(pulseWidth, 2500, 5000, 180, 360);
  }

  lcd.setCursor(0, 1);
  lcd.print("Angle: ");
  lcd.print(angle);
}

void displayEscPulseWidthAndRPM() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pulse Width: ");
  lcd.print(escPulseWidth);
  lcd.print(" us");

  // Calculate the corresponding RPM
  int rpm;
  if (escPulseWidth < 500) {
    rpm = map(escPulseWidth, 0, 1000, -1000, 0);
  } else if (escPulseWidth <= 2500) {
    rpm = map(escPulseWidth, 1000, 2000, 0, 10000);
  } else {
    rpm = map(escPulseWidth, 2000, 5000, 10000, 20000);
  }

  lcd.setCursor(0, 1);
  lcd.print("RPM: ");
  lcd.print(rpm);
}