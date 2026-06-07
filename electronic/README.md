# Electronics

This folder contains the electrical documentation of the autonomous Connect Four robot.

The electronics system is responsible for controlling the robot movement, detecting the player moves, actuating the piece release mechanism, and connecting all sensors and actuators to the main controller.

## Current contents

At this stage, this folder includes:

- `ESP32_Datasheet.pdf`  
  Official datasheet of the ESP32 development board used as the main controller.

- `easyeda_schematic.pdf` / `easyeda_schematic.png`  
  Electrical schematic designed using EasyEDA.

## System overview

The electrical system is based on an ESP32 microcontroller, which manages the main inputs and outputs of the robot.

The main connected elements are:

- ESP32 development board
- IR sensors for detecting the column used by the human player
- Limit switch for homing and position reference
- Stepper motor driver for horizontal movement
- NEMA17 stepper motor
- 9g servo motor for the piece release mechanism
- External power supply
- Common ground connection between control and power circuits

## Main functions

The electronics system performs the following tasks:

1. Read the IR sensors to detect where the player drops a piece.
2. Use the limit switch to calibrate the initial position of the carriage.
3. Control the stepper motor driver to move the dispenser horizontally.
4. Activate the servo motor to release a game piece.
5. Provide the electrical interface between the software, sensors, and actuators.

## Electrical schematic

The schematic was created with EasyEDA and shows the connection between the ESP32, sensors, actuator signals, driver inputs, and power lines.

The schematic includes:

- ESP32 pin connections
- Sensor signal inputs
- Servo signal output
- Stepper driver control pins
- Power supply connections
- Ground reference connections

## Notes

All components must share a common ground to ensure correct signal reference between the ESP32, sensors, servo motor, and motor driver.

The ESP32 operates with 3.3 V logic, so all input and output signals must be compatible with this voltage level. External power is required for the stepper motor and may also be required for the servo motor, depending on current consumption.

## Files to be added

The following files should be added in future updates:

- Final version of the electrical schematic in PDF format
- EasyEDA source file
- Wiring diagram
- Pinout table
- Bill of Materials for electronics
- Power distribution diagram
- Photos of the real wiring
- Datasheets of the main electronic components
- Short explanation of the final electrical assembly
- Safety notes and electrical precautions

## Recommended folder structure

```text
electronics/
│
├── README.md
├── datasheets/
│   ├── ESP32_Datasheet.pdf
│   ├── TMC2209_Datasheet.pdf
│   ├── NEMA17_Datasheet.pdf
│   ├── Servo_9g_Datasheet.pdf
│   └── IR_Sensor_Datasheet.pdf
│
├── schematics/
│   ├── easyeda_schematic.pdf
│   ├── easyeda_schematic.png
│   └── easyeda_project_file.json
│
├── wiring/
│   ├── wiring_diagram.pdf
│   └── pinout_table.md
│
└── photos/
    ├── electronics_assembly.jpg
    ├── esp32_connections.jpg
    └── final_wiring.jpg
