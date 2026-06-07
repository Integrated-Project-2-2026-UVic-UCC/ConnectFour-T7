<div align="center">

# Autonomous Connect 4 Machine
### Integrated Project II · Mechatronics Engineering · UVic-UCC 2026

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?logo=c%2B%2B)](https://isocpp.org/)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-E7352C?logo=espressif)](https://www.espressif.com/)
[![Status: Integration & Testing](https://img.shields.io/badge/Status-Integration%20%26%20Testing-yellow)]()
[![University: UVic-UCC](https://img.shields.io/badge/University-UVic--UCC-darkgreen)]()

*A fully autonomous physical Connect 4 machine: the ESP32 detects each piece drop with photoelectric sensors, decides the best move, and physically delivers the piece using a motorized lead screw dispenser.*

</div>

---

## Table of Contents

- [About the Project](#about-the-project)
- [How It Works](#how-it-works)
- [Mechanical Design](#mechanical-design)
- [Electronics](#electronics)
- [Software](#software)
- [Hardware Components](#hardware-components)
- [Getting Started](#getting-started)
- [Current Status](#current-status)
- [Team](#team)
- [License](#license)

---

## About the Project

This project is developed as part of **Integrated Project II** in the **Mechatronics Engineering** degree at the Universitat de Vic – Universitat Central de Catalunya (UVic-UCC).

The goal is to build a machine that plays **Connect 4 autonomously** against a human opponent. The machine detects the human's move through **photoelectric sensors**, decides which column to play, moves a **3D-printed piece dispenser** along a lead screw to the correct position, and drops a piece with a **servo motor** — all controlled by an **ESP32**.

The robot does not use an artificial intelligence engine. Instead, it follows a predefined logical strategy: win if possible, block the opponent, or otherwise play an available column. All game communication is handled through the **Serial Monitor**.

---

## How It Works
<div align="center">
```
Human drops a piece
        │
        ▼
Photoelectric sensor (one per column)
detects the piece passing through
        │
        ▼
ESP32 updates the board state
        │
        ▼
Software selects the column to play
(win > block > random available column)
        │
        ▼
Stepper motor moves the dispenser
along the lead screw to the target column
        │
        ▼
Servo motor pushes the ramp and releases one piece
        │
        ▼
Piece falls into the board
        │
        ▼
Game state shown through the Serial Monitor
```

> **Note:** Piece recovery and board reset are performed manually between games.

---

## Mechanical Design

The mechanical design is organized into two main assemblies, all modeled in PTC Creo, exported as STL for 3D printing, and documented with technical drawings.

**Board structure assembly** — represents the game board and holds the structure upright:
- **Foot L** and **Foot R** — the two legs of the structure
- **Bottom Cover** — closes the structure and assembles onto the base
- **Connect 4** — the main board frame with the 7 × 6 grid of columns
- **Connect 4 Cover** — the front cover that closes the playing grid

**Dispenser and linear-motion assembly** — carries and releases the pieces:
- **Support Foot** and **Dispenser Support Bar** — hold the dispenser in place
- **Motor Support** — holds the stepper motor at one end of the lead screw
- **Bearing Support** — holds the bearing at the opposite end of the lead screw
- **Dispenser Part 1** and **Dispenser Part 2** — form the dispenser body

The pieces are stored horizontally inside the dispenser. To drop a piece, an internal ramp reorients it from horizontal to vertical so it falls correctly into the selected column, and a servo motor pushes the ramp downward to release one piece at a time. The whole dispenser slides along the lead screw to align with any of the seven columns.

---

## Electronics

The electrical system is based on an **ESP32** microcontroller, which manages all inputs and outputs of the robot:
- Reads the photoelectric (IR) sensors to detect the column played by the human
- Controls the stepper motor driver to move the dispenser horizontally
- Activates the servo motor to release a piece
- Provides the electrical interface between software, sensors, and actuators

The ESP32 works with 3.3 V logic, and an external power supply feeds the stepper motor and servo. All components share a common ground to keep a correct signal reference. The schematic was designed with EasyEDA.

---

## Software

- **Microcontroller:** ESP32
- **Language:** C++ (Arduino)
- **Key modules:**
  - `detection` — reads photoelectric sensors and maps them to a column index
  - `motion` — stepper step generation and position control
  - `servo` — piece release timing and control
  - `game` — board state, win/block detection, move selection
  - `serial` — Serial Monitor communication (prompts and game status)

The board is handled as a 6-row by 7-column matrix. The code initializes the board, manages player and robot moves, validates that columns are not full, and detects winning conditions (horizontal, vertical, and diagonal) as well as draws.

Two final versions of the system are provided:
- **Automatic Detection Mode** — the player's move is detected automatically by the photoelectric sensors, which identify the played column.
- **Manual Input Mode** — a backup version where the player enters the column number through the Serial Monitor, in case the sensors fail.

---

## Hardware Components

| Component | Description | 
|-----------|-------------|
| ESP32 | Main microcontroller — game logic, motor and sensor control |
| NEMA17 stepper motor | X-axis movement via lead screw |
| Stepper driver | Motor driver for step/direction control |
| Lead screw | Linear X-axis mechanism |
| Linear guide + carriage | Supports and guides the dispenser |
| 9g micro servo | Releases one piece at a time from the dispenser |
| Photoelectric / IR sensors | One per column, detect the played column |
| Series resistors | Required for reliable photoelectric detection |
| External power supply | Powers the stepper motor and servo |
| Connect 4 board | 7 × 6 grid with sensor holes at the top |

---

## Getting Started

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/) with ESP32 board support
- ESP32 board package installed
- Required libraries:
  - `AccelStepper` — stepper motor control
  - `ESP32Servo` — servo control

### Flash to ESP32

1. Open the project in Arduino IDE or PlatformIO
2. Select board: **ESP32 Dev Module**
3. Connect ESP32 via USB
4. Upload

> **Warning:** Make sure the sensors, motor, and servo are connected before powering on, and that the external power supply for the motor and servo is on.

---

## Current Status

| Phase | Task | Status |
|-------|------|--------|
| 1 | Initial design and planning | Done |
| 2 | Component procurement | Done |
| 3 | 3D printing — dispenser and mechanical parts | Done |
| 3 | Electronics wiring and assembly | Done |
| 3 | Individual component testing (sensors, motor, servo) | Done |
| 4 | Full system integration | Done |
| 4 | Software validation on real hardware | Done |
| 5 | Final calibration and demo | Done |

---

## Team

| Name | Role |
|------|------|
| Arnau Arcarons | Project Manager |
| Pau Vila | Mechanical Leader |
| Victor Ruiz | Electronics Leader |
| Albert Llimós | Programming Leader |

> Project supervised by Moisès and Clara, faculty of Mechatronics Engineering at UVic-UCC.

---

## License

This project is licensed under the **GNU General Public License v3.0**.
See the [`LICENSE`](LICENSE) file for full details.



