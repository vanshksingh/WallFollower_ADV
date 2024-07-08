

#Introduction

This project involves using an Arduino board to control two stepper motors and an LCD display. The system utilizes ultrasonic sensors to detect obstacles and navigate according to the chosen wall-following strategy. Additionally, the system provides sound feedback using a buzzer.


![IMG_3105](https://github.com/vanshksingh/WallFollower_ADV/assets/114809624/f991d2fc-c70e-4e9e-9f0d-efe2cd5b1544)

## Components

The project requires the following hardware components:
- Arduino board
- Two stepper motors
- Stepper motor drivers
- LCD display (LiquidCrystal_I2C)
- Ultrasonic sensors (trig and echo pins for each sensor)
- Buzzer
- Push buttons
- Various resistors and connections

## Installation and Setup

1. **Circuit Setup:**
   - Connect the stepper motors and their drivers to the appropriate pins as defined in the code (DIR_X, STEP_X, DIR_Y, STEP_Y).
   - Connect the ultrasonic sensors to the pins defined in the code (TRIG_PIN_A, ECHO_PIN_A, etc.).
   - Connect the LCD display and set up according to the code (LiquidCrystal_I2C lcd(0x27, 16, 2)).
   - Connect the buzzer and the push buttons to the specified pins (buzzerPin, buttonPinA1, buttonPinA2).
   - Make sure to have proper connections and wiring as per the schematic.

2. **Software Installation:**
   - Ensure you have the Arduino IDE installed.
   - Clone or download this project repository.
   - Open the project in the Arduino IDE.
   - Verify you have the required libraries installed (LiquidCrystal_I2C, BasicStepperDriver).
   - Upload the sketch to the Arduino board.

3. **Addtional Reversed Engineered Schematics***
  ![Screenshot 2024-04-23 at 4 53 56 AM](https://github.com/vanshksingh/WallFollower_ADV/assets/114809624/7cfed798-3da8-419a-8326-b5635447af45)
![Screenshot 2024-04-23 at 4 53 58 AM](https://github.com/vanshksingh/WallFollower_ADV/assets/114809624/aaeb5b35-0653-4872-8cef-1b09b5960ea5)


   


## How to Use

1. **Operation:**
   - After powering up, the system will display a startup message and play a startup sound.
   - The system waits for input from the push buttons.
   - Press button A1 to follow the right wall or button A2 to follow the left wall.
   - The system will then run in the chosen direction, following the wall while avoiding obstacles using ultrasonic sensors.
   - The system provides visual feedback on the LCD and sound feedback through the buzzer.

2. **Interaction:**
   - Press button A1 to follow the right wall.
   - Press button A2 to follow the left wall.
   - The system can be paused and restarted by pressing either button during operation.

3. **Troubleshooting:**
   - Check wiring and connections if the system does not respond as expected.
   - Ensure all libraries are correctly installed and the code is properly uploaded to the Arduino board.

## Notes

- Adjust the parameters in the code (e.g., motor speed, sensor threshold) according to your specific hardware setup for optimal performance.
- The code includes functions for sound feedback, LCD display updates, and obstacle detection.
- The code is organized into setup and loop functions, with additional helper functions for specific tasks.

![IMG_4166](https://github.com/vanshksingh/WallFollower_ADV/assets/114809624/04e58192-100e-4501-bfc2-3aa33e7aa55a)


![IMG_4188](https://github.com/vanshksingh/WallFollower_ADV/assets/114809624/0b1cea40-b62f-4f31-8c10-24049f1b713a)


## Conclusion

This project demonstrates how to integrate stepper motor control, ultrasonic sensors, an LCD display, and sound feedback into an Arduino project. It can be customized for different applications, such as robotic navigation or obstacle avoidance systems.
