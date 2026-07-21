An interactive Pomodoro robot that dispenses sweet rewards only if you stay focused at your desk.

## Materials 

- Servo
- Ultrasonic sensor
- SSD1306 0.96 inch I2C OLED display 
- Touch Sensor TTP223 
- Passive Buzzer

## How to Compile 

1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software](https://www.arduino.cc/).
2. Install required libraries via Library Manager:
   - 'Adafruit GFX'
   - 'Adafruit SSD1306'
3. Open 'Code/Pomodoro_Robot/Pomodoro_Robot.ino' in Arduino IDE.
4. Select board: *Tools > Board > Arduino AVR Boards > Arduino Nano*.
5. Connect the Arduino Nano via USB, choose the correct COM Port, and click *Upload*.

## 3D Printing

The chassis and structural components for the Pomodoro Robot are entirely 3D printed. You can find all the ready-to-print '.STL' files along with the '.STEP' exports organized in the 'Hardware/Design/STL_Files' and 'Hardware/Design/STEP_Files' directories.
