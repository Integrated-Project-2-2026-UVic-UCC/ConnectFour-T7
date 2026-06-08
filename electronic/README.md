# Electronics

This folder contains the final electronic documentation of the autonomous Connect Four robot developed by Team 7 for the Integrated Projects II course at UVic-UCC.

The electronic system connects the control software with the physical hardware of the machine. It is responsible for reading the player move sensors, controlling the stepper motor driver, activating the piece dispenser servo, and managing the basic electrical interface between all components.

---

## Folder Contents

This folder includes the main electrical schematic of the prototype and the datasheets of the key electronic components used in the system.

| File | Description |
|---|---|
| `IP_2.pdf` | Final electrical schematic of the system, created with EasyEDA. It shows the connections between the ESP32 controller, sensors, stepper driver, servo motor, power supply and common ground. |
| `A1.3 esp32-s3_datasheet_en.pdf` | Datasheet of the ESP32-S3 microcontroller, used as the main controller reference for GPIO, electrical characteristics and pin functions. |
| `A4988.PDF` | Datasheet/reference document for the A4988 stepper motor driver used to control the NEMA17 stepper motor. |
| `README.md` | Documentation file describing the purpose and contents of the electronics folder. |

---

## Electronic System Overview

The electronic system is centred on the ESP32 microcontroller. The ESP32 receives input signals from the column sensors and sends control signals to the actuators of the machine.

The system performs the following main tasks:

1. Detecting the column selected by the human player.
2. Sending STEP and DIR signals to the A4988 stepper motor driver.
3. Moving the NEMA17 stepper motor along the horizontal axis.
4. Activating the 9 g servo motor to release one piece.
5. Reading the limit switch used as a mechanical reference.
6. Coordinating the electrical behaviour of the full prototype.

---

## Main Electronic Components

| Component | Function | Interface |
|---|---|---|
| ESP32 / ESP32-S3 | Main controller of the system | GPIO / PWM / Serial |
| A4988 stepper motor driver | Controls the NEMA17 stepper motor | STEP / DIR |
| NEMA17 stepper motor | Moves the dispenser horizontally | Driven by A4988 |
| 9 g servo motor | Opens and closes the dispenser gate | PWM |
| Column sensors | Detect the column used by the human player | Digital inputs |
| Limit switch | Provides a reference position for the axis | Digital input |
| External power supply | Powers the motor and actuator system | Power input |
| Common ground | Shared electrical reference | GND |

---

## Electrical Schematic

The electrical schematic is provided in:

```text
IP_2.pdf
