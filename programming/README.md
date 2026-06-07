## Project Description

This project implements the control software for a robot capable of playing the Connect Four game. The program acts as the robot's control system, managing the game logic, player interaction, and the robot's decision-making process.

The system allows a complete game between the player and the robot by validating moves, managing turns, and determining the final result (win, loss, or draw).

## Project Structure

During the development process, the project was divided into three main parts:

1. Robot decision-making
2. Player interaction
3. Mechanical control and piece launching

In the final version, the implementation of an artificial intelligence system was not considered necessary. Instead, the robot follows a predefined logical strategy to make its decisions.

## Code Functionality

The code manages a 6-row by 7-column matrix that represents the game board. The main functionalities include:

* Board initialization
* Management of player and robot moves
* Validation of moves to ensure that columns are not full
* Detection of winning conditions (horizontal, vertical, and diagonal)
* Detection of draw situations when the board is full

## Robot Decision Logic

The robot follows a simple three-step strategy:

1. Check for an immediate winning move
2. Block the player if they are about to win
3. Select a random available column if no better option is found

## Player Interaction

The LCD display originally planned for the project was not implemented. Instead, all communication with the player is performed through the computer's Serial Monitor.

The serial interface allows the user to:

* Select which player starts the game
* Enter player moves
* Display game status messages
* Restart the game after a match ends

## Sensor and Mechanical Testing

Several tests were carried out to validate the operation of the photoelectric sensors. Although the sensors work correctly in most situations, occasional detection failures were observed.

For this reason, the development process was divided into several independent test programs:

* Sensor testing code
* Stepper motor movement testing code
* Servo-based piece launching testing code
* Main logic code that integrates player interaction and the robot's decision-making strategy

## Final Implementations

Two final versions of the system were developed:

### Automatic Detection Mode

In this version, the player's move is detected automatically by a photoelectric sensor, which identifies the column where the piece has been dropped. The robot then analyzes the board and performs its move according to the predefined logic.

### Manual Input Mode

Since the sensors may occasionally fail, a backup version was also developed. In this mode, the player enters the column number through the Serial Monitor after each move. The robot then updates the board, analyzes the game state, and performs its move using the same logical decision-making process.

## Current Project Status

The project has reached a functional state where:

* The robot can play a complete game of Connect Four
* Player interaction is managed through the Serial Monitor
* The mechanical launching system has been tested
* Sensor detection has been implemented and evaluated
* A backup manual mode ensures reliable operation even if the sensors fail




