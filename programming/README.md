
## Project Description

This project implements the control code of a robot capable of playing the Connect Four game. At the current stage of development, the program mainly acts as the “brain” of the robot, responsible for decision-making and game management.

The system allows a complete simulation of a match between the player and the robot, handling turns, validating moves, and determining the final result (win, loss, or draw).

## Code Functionality

The code is structured to control a 6-row by 7-column matrix that represents the game board. The main functionalities include:

* Initialization of the board with empty positions
* Management of player and robot moves
* Validation of moves to ensure columns are not full
* Detection of winning conditions (horizontal, vertical, and diagonal)
* Detection of draw situations when the board is full

## Robot Decision Logic

The robot follows a basic decision-making strategy based on three levels:

1. Attempt to win by checking if there is a move that leads to an immediate victory
2. Block the player if they are about to win
3. Select a random available column if no better option is found

At this stage, no advanced artificial intelligence system has been implemented.

## Player Interaction

The system includes interaction through the serial interface, allowing:

* Selection of which player starts the game
* Input of player moves
* Display of messages during the game
* Option to restart the game after it ends

In future versions, this interaction is planned to be implemented using an LCD screen.

## Current Project Status

The project is currently in an intermediate development phase:

* The code mainly focuses on game logic and robot decision-making
* Motor testing is currently in progress
* The system is waiting for the arrival of photoelectric sensors
* Mechanical development is being carried out in parallel, especially the piece launching mechanism

## Future Work

Once all components are available, the next steps will include:

* Final system assembly
* Integration of sensors for piece detection
* Adjustment and optimization of motor movements for accurate piece launching
* Implementation of an LCD-based interface
* Possible improvements to the robot logic, including artificial intelligence features





