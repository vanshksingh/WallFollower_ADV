#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "BasicStepperDriver.h"
#include "SyncDriver.h"

#define MOTOR_STEPS 200
#define MOTOR_X_RPM 130
#define MOTOR_Y_RPM 130
#define MICROSTEPS 16

#define DIR_X 6
#define STEP_X 3
#define DIR_Y 5
#define STEP_Y 2

#define TRIG_PIN_A 22
#define ECHO_PIN_A 23
#define TRIG_PIN_CENTER 24
#define ECHO_PIN_CENTER 25
#define TRIG_PIN_B 26
#define ECHO_PIN_B 27

#define TRIG_PIN_RIGHT 18
#define ECHO_PIN_RIGHT 19
#define TRIG_PIN_LEFT 16
#define ECHO_PIN_LEFT 17

BasicStepperDriver stepperX(MOTOR_STEPS, DIR_X, STEP_X);
BasicStepperDriver stepperY(MOTOR_STEPS, DIR_Y, STEP_Y);
SyncDriver controller(stepperX, stepperY);

unsigned long lastWallDetectionTime = 0;

void playSoundLoop(int tones[], int size);
void playCountdownWithStart();
void driver(int dir);
void RUN(int dir);
float getDistance(int trigPin, int echoPin);

const int buzzerPin = A8; // Positive pin
const int buzzerNegativePin = A11; // Negative pin

// Define the pins for the buttons
const int buttonPinA1 = A1;
const int buttonPinA2 = A2;

// Define variables to store the state of the buttons
int buttonStateA1 = 0;
int buttonStateA2 = 0;

// Define the sounds for each button
int sciFiTonesA1[] = {1000, 1500, 2000, 2500, 3000, 3500};
int sciFiTonesA2[] = {200, 400, 600, 800, 1000, 1200}; // Distinctive tones for button A2
int duration = 100; // Duration of each tone in milliseconds

// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Define the LCD dimensions
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  pinMode(TRIG_PIN_A, OUTPUT);
  pinMode(ECHO_PIN_A, INPUT);
  pinMode(TRIG_PIN_CENTER, OUTPUT);
  pinMode(ECHO_PIN_CENTER, INPUT);
  pinMode(TRIG_PIN_B, OUTPUT);
  pinMode(ECHO_PIN_B, INPUT);

  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);

  stepperX.begin(MOTOR_X_RPM, MICROSTEPS);
  stepperY.begin(MOTOR_Y_RPM, MICROSTEPS);
  stepperX.setEnableActiveState(LOW);
  stepperY.setEnableActiveState(LOW);

  // Set the button pins as inputs with internal pull-ups
  pinMode(buttonPinA1, INPUT_PULLUP);
  pinMode(buttonPinA2, INPUT_PULLUP);

  // Set the buzzer pins as outputs
  pinMode(buzzerPin, OUTPUT);
  pinMode(buzzerNegativePin, OUTPUT);
  digitalWrite(buzzerNegativePin, LOW);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting Systems");
  lcd.setCursor(0, 1);
  lcd.print("  Processing..");
  delay(300);

  // Play startup sound
  int startupSound[] = {1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800}; // Sci-fi startup sound
  playSoundLoop(startupSound, sizeof(startupSound) / sizeof(startupSound[0]));
  delay(300);
}

void loop() {
  lcd.setBacklight(LOW); // Turn off backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Operation PAUSED");
  lcd.setCursor(0, 1);
  lcd.print("Right ^^|,, LEFT");
  delay(250);
  lcd.setBacklight(HIGH); // Turn on backlight
  delay(700);

  

  // Read the state of the buttons (LOW when pressed, HIGH when not pressed)
  buttonStateA1 = digitalRead(buttonPinA1);
  buttonStateA2 = digitalRead(buttonPinA2);

  // Check if button A1 is clicked
  if (buttonStateA1 == LOW) {
    driver(1);
  }

  // Check if button A2 is clicked
  if (buttonStateA2 == LOW) {
    driver(0);
  }
}

void driver(int dir) {
  while (true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (dir == 0) {
      lcd.print("RIGHT WALL");
      lcd.setCursor(0, 1);
      lcd.print("SELECTED");
      playSoundLoop(sciFiTonesA1, sizeof(sciFiTonesA1) / sizeof(sciFiTonesA1[0])); // Play sound loop for button A1
    }
    if (dir == 1) {
      lcd.print("LEFT WALL");
      lcd.setCursor(0, 1);
      lcd.print("SELECTED");
      playSoundLoop(sciFiTonesA2, sizeof(sciFiTonesA2) / sizeof(sciFiTonesA2[0])); // Play sound loop for button A2
    }
    delay(500);
    playCountdownWithStart();
    RUN(dir);
    break;
  }
}

void RUN(int dir) {
  while (dir == 0) {
    float distanceA = getDistance(TRIG_PIN_A, ECHO_PIN_A);
    float distanceCenter = getDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER);
    float distanceB = getDistance(TRIG_PIN_B, ECHO_PIN_B);

    float distanceRight = getDistance(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
    float distanceLeft = getDistance(TRIG_PIN_LEFT, ECHO_PIN_LEFT);


    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Right Wall");



    if (distanceCenter < 10 || distanceA < 10 || distanceB < 10 || distanceRight < 6 || distanceLeft < 6) {
        // Obstacle detection
        

        bool spaceRight = distanceRight > 6; // Check if there's space on the right
        bool spaceLeft = distanceLeft > 6;   // Check if there's space on the left

        if (spaceRight && spaceLeft) {
            // If space available on both sides, turn towards the side with more space
            if (distanceRight > distanceLeft) {
            
              lcd.setCursor(0,0);
              lcd.print("Turn Right");
                // Turn right
                controller.rotate(50, -50);
            } else {
             
              lcd.setCursor(0,0);
              lcd.print("Turn Left");
                // Turn left
                controller.rotate(-50, 50);
            }
        } else if (spaceRight) {
          
              lcd.setCursor(0,0);
              lcd.print("Turn Right");
            // If only space available on the right, turn right
            controller.rotate(50, -50);
        } else if (spaceLeft) {
      
              lcd.setCursor(0,0);
              lcd.print("Turn Left");
            // If only space available on the left, turn left
            controller.rotate(-50, 50);
        } else {
         
              lcd.setCursor(0,0);
              lcd.print("RETURN");
            controller.rotate(-100, 100);
        }

        
    } else {
      if(distanceRight >= 10){
        
              lcd.setCursor(0,0);
              lcd.print("Correcting...");
      controller.rotate(50,20);
    }
    else{
      
      
              lcd.setCursor(0,0);
              lcd.print("Forward");
              // If no obstacle detected, move forward

        controller.rotate(50, 50);}
        
    }

    

    // Check if any button is clicked to stop the loop
    buttonStateA1 = digitalRead(buttonPinA1);
    buttonStateA2 = digitalRead(buttonPinA2);
    if (buttonStateA1 == LOW || buttonStateA2 == LOW) {
      break;
    }
  }



  while (dir == 1) {
    float distanceA = getDistance(TRIG_PIN_A, ECHO_PIN_A);
    float distanceCenter = getDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER);
    float distanceB = getDistance(TRIG_PIN_B, ECHO_PIN_B);

    float distanceRight = getDistance(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
    float distanceLeft = getDistance(TRIG_PIN_LEFT, ECHO_PIN_LEFT);


    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Left Wall");



    if (distanceCenter < 10 || distanceA < 10 || distanceB < 10 || distanceRight < 6 || distanceLeft < 6) {
        // Obstacle detection
        

        bool spaceRight = distanceRight > 6; // Check if there's space on the right
        bool spaceLeft = distanceLeft > 6;   // Check if there's space on the left

        if (spaceRight && spaceLeft) {
            // If space available on both sides, turn towards the side with more space
            if (distanceRight > distanceLeft) {
              
              lcd.setCursor(0,0);
              lcd.print("Turn Right");
                // Turn right
                controller.rotate(50, -50);
            } else {
              
              lcd.setCursor(0,0);
              lcd.print("Turn Left");
                // Turn left
                controller.rotate(-50, 50);
            }
        } else if (spaceRight) {
          
              lcd.setCursor(0,0);
              lcd.print("Turn Right");
            // If only space available on the right, turn right
            controller.rotate(50, -50);
        } else if (spaceLeft) {
          
              lcd.setCursor(0,0);
              lcd.print("Turn Left");
            // If only space available on the left, turn left
            controller.rotate(-50, 50);
        } else {
          
              lcd.setCursor(0,0);
              lcd.print("RETURN");
            controller.rotate(-100, 100);
        }

        
    } else {
      if(distanceLeft >= 10){
        
              lcd.setCursor(0,0);
              lcd.print("Correcting...");
      controller.rotate(20,50);
    }
    else{
      
      
              lcd.setCursor(0,0);
              lcd.print("Forward");
              // If no obstacle detected, move forward

        controller.rotate(50, 50);}
        
    }

    

    // Check if any button is clicked to stop the loop
    buttonStateA1 = digitalRead(buttonPinA1);
    buttonStateA2 = digitalRead(buttonPinA2);
    if (buttonStateA1 == LOW || buttonStateA2 == LOW) {
      break;
    }
  }
}



void playSoundLoop(int tones[], int size) {
  for (int i = 0; i < size; i++) {
    tone(buzzerPin, tones[i]);
    delay(duration);
    noTone(buzzerPin);
    delay(50); // Pause between tones
  }
}

float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) / 58.0; // Convert pulse duration to distance in cm
}

void playCountdownWithStart() {
  int countdownSound[] = {1500, 1300, 1100}; // Countdown sound (3 tones)

  for (int i = 0; i < sizeof(countdownSound) / sizeof(countdownSound[0]); i++) {
    int centerCol = (LCD_COLS - 1) / 2 - 1;

  lcd.setBacklight(HIGH); // Turn on backlight
    lcd.clear();
    lcd.setCursor(centerCol, 0);
    lcd.print(3 - i);

    tone(buzzerPin, countdownSound[i]);
    delay(700);
    noTone(buzzerPin);
    lcd.setBacklight(LOW); // Turn off backlight
  delay(500);
  }
lcd.setBacklight(HIGH); // Turn on backlight
  lcd.clear();
  int startTextLength = 6; // Length of "START!"
  int startTextCol = (LCD_COLS - startTextLength) / 2;
  lcd.setCursor(startTextCol, 0);
  lcd.print("START!");
}
