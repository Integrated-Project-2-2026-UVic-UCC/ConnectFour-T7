// CONNECT 4 - FUSED CODE (NO SENSORS)
// Game logic + hardware (stepper motor + servo dispenser)
// Player enters the column they played through the Serial Monitor.
// Team 7 - UVic-UCC - Projecte Integrat II - 2026
// WARNING: place the dispenser on COLUMN 7 by hand before powering on.
// Libraries: AccelStepper (Mike McCauley) - ESP32Servo (Kevin Harrington)

#include "esp_sleep.h"
#include "soc/soc.h"           // needed to disable the brownout detector
#include "soc/rtc_cntl_reg.h"  // needed to disable the brownout detector
#include <AccelStepper.h>
#include <ESP32Servo.h>

// PINS
#define PIN_STEP   17    // A4988 STEP signal
#define PIN_DIR    16    // A4988 DIR signal
#define PIN_SERVO   4    // dispenser servo signal

// MECHANICAL SETTINGS
#define STEPS_PER_MM       25    // 200 steps/rev divided by 8 mm lead
#define COLUMN_SPACING_MM  32    // distance in mm between two columns
#define SPEED             800    // steps per second
#define ACCEL             400    // steps per second squared

// SERVO ANGLES
#define SERVO_CLOSED  20    // gate closed (piece is held back)
#define SERVO_OPEN    80    // gate open   (piece drops)
#define SERVO_TIME   600    // ms the gate stays open

// OBJECTS
AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);
Servo servo;

// GAME VARIABLES
char board[6][7];               // game board, '-' means empty
const int rows = 6;             // number of rows
const int cols = 7;             // number of columns
const char player = 'X';        // human player token
const char robot  = 'O';        // machine token
bool playerTurn;                // true = player's turn, false = robot's turn

// HARDWARE FUNCTIONS

// Steps needed to reach a given column (1-7). Column 7 is position 0.
long stepsForColumn(int col) {
  return (long)(7 - col) * COLUMN_SPACING_MM * STEPS_PER_MM;
}

// Move the dispenser to the requested column (1-7)
void moveToColumn(int col) {
  Serial.print(F("Motor -> column "));
  Serial.println(col);

  motor.setMaxSpeed(SPEED);
  motor.setAcceleration(ACCEL);
  motor.moveTo(stepsForColumn(col));
  while (motor.distanceToGo() != 0) motor.run();  // block until target reached
}

// Open the servo gate to release one piece, then close it again
void dispense() {
  delay(500);
  servo.write(SERVO_OPEN);   // open the gate
  delay(SERVO_TIME);         // keep it open so the piece falls
  servo.write(SERVO_CLOSED); // close the gate
  delay(300);
  Serial.println(F("Piece dispensed."));
}

// BOARD FUNCTIONS

// Fill the whole board with empty cells
void initBoard() {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      board[i][j] = '-';
}

// Print the current board to the Serial Monitor
void showBoard() {
  Serial.println();
  Serial.println(F("+---+---+---+---+---+---+---+"));
  for (int i = 0; i < rows; i++) {
    Serial.print(F("| "));
    for (int j = 0; j < cols; j++) {
      Serial.print(board[i][j]);
      Serial.print(F(" | "));
    }
    Serial.println();
    Serial.println(F("+---+---+---+---+---+---+---+"));
  }
  Serial.println(F("  1   2   3   4   5   6   7  "));
  Serial.println();
}

// Drop a token into a column (1-7). Returns false if the column is full.
bool dropToken(int column, char token) {
  column--;                                          // convert to 0-based index
  if (column < 0 || column >= cols) return false;    // out of range
  for (int i = rows - 1; i >= 0; i--) {              // search from the bottom up
    if (board[i][column] == '-') {
      board[i][column] = token;
      return true;
    }
  }
  return false;                                      // column already full
}

// Check whether a token has 4 in a row (any direction)
bool checkWin(char token) {
  // horizontal
  for (int i = 0; i < rows; i++)
    for (int j = 0; j <= cols - 4; j++)
      if (board[i][j]   == token && board[i][j+1] == token &&
          board[i][j+2] == token && board[i][j+3] == token) return true;
  // vertical
  for (int i = 0; i <= rows - 4; i++)
    for (int j = 0; j < cols; j++)
      if (board[i][j]   == token && board[i+1][j] == token &&
          board[i+2][j] == token && board[i+3][j] == token) return true;
  // diagonal down-right
  for (int i = 0; i <= rows - 4; i++)
    for (int j = 0; j <= cols - 4; j++)
      if (board[i][j]     == token && board[i+1][j+1] == token &&
          board[i+2][j+2] == token && board[i+3][j+3] == token) return true;
  // diagonal down-left
  for (int i = 3; i < rows; i++)
    for (int j = 0; j <= cols - 4; j++)
      if (board[i][j]     == token && board[i-1][j+1] == token &&
          board[i-2][j+2] == token && board[i-3][j+3] == token) return true;
  return false;
}

// Return true when the top row is full (board is full)
bool boardFull() {
  for (int j = 0; j < cols; j++)
    if (board[0][j] == '-') return false;
  return true;
}

// ROBOT AI (win > block > random available column)

int robotLogic() {
  int c, row;

  // 1. take the winning move if one exists
  for (c = 1; c <= cols; c++) {
    if (board[0][c-1] != '-') continue;              // column full
    row = -1;
    for (int i = rows-1; i >= 0; i--)
      if (board[i][c-1] == '-') { row = i; break; }  // lowest free cell
    if (row == -1) continue;
    board[row][c-1] = robot;                         // simulate the move
    bool win = checkWin(robot);
    board[row][c-1] = '-';                           // undo the simulation
    if (win) return c;
  }

  // 2. block the player if they are about to win
  for (c = 1; c <= cols; c++) {
    if (board[0][c-1] != '-') continue;
    row = -1;
    for (int i = rows-1; i >= 0; i--)
      if (board[i][c-1] == '-') { row = i; break; }
    if (row == -1) continue;
    board[row][c-1] = player;                        // simulate player's move
    bool win = checkWin(player);
    board[row][c-1] = '-';
    if (win) return c;
  }

  // 3. otherwise pick a random available column
  int tries = 0;
  do {
    c = random(1, cols + 1);
    tries++;
    if (tries > 100) return -1;                      // no column available
  } while (board[0][c-1] != '-');

  return c;
}

// MATCH CONTROL

// Ask the player whether they want to play again (S = yes, N = no)
bool askRestart() {
  Serial.println(F("Play again? (S/N)"));
  while (true) {
    if (Serial.available() > 0) {
      char r = Serial.read();
      if (r == 'S' || r == 's') return true;
      if (r == 'N' || r == 'n') return false;
    }
  }
}

// Handle the end of a match: print the result and either restart or sleep
void endMatch(const char* message) {
  Serial.println();
  Serial.println(F("============================"));
  Serial.println(message);
  Serial.println(F("============================"));
  delay(2000);

  if (askRestart()) {
    // the dispenser must be put back on column 7 by hand
    Serial.println(F("Put the dispenser back on column 7 and press S."));
    while (true) {
      if (Serial.available() > 0) {
        char r = Serial.read();
        if (r == 'S' || r == 's') break;
      }
    }
    initBoard();
    motor.setCurrentPosition(0);   // reset: dispenser is on column 7

    Serial.println(F("Do you want to start? (S/N)"));
    while (true) {
      if (Serial.available() > 0) {
        char r = Serial.read();
        if (r == 'S' || r == 's') { playerTurn = true;  Serial.println(F("You start.")); break; }
        if (r == 'N' || r == 'n') { playerTurn = false; Serial.println(F("Robot starts.")); break; }
      }
    }
    showBoard();
  } else {
    Serial.println(F("See you soon!"));
    esp_deep_sleep_start();        // put the ESP32 to sleep
  }
}

// SETUP

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // disable the brownout detector
  Serial.begin(9600);
  randomSeed(esp_random());

  // motor: current position = column 7 (placed by hand)
  motor.setMaxSpeed(SPEED);
  motor.setAcceleration(ACCEL);
  motor.setCurrentPosition(0);

  // servo: start with the gate closed
  servo.attach(PIN_SERVO);
  servo.write(SERVO_CLOSED);

  Serial.println(F("============================"));
  Serial.println(F("  CONNECT 4 - TEAM 7 UVIC  "));
  Serial.println(F("============================"));
  Serial.println(F("The dispenser must be on column 7."));

  initBoard();

  Serial.println(F("Do you want to start? (S/N)"));
  while (true) {
    if (Serial.available() > 0) {
      char r = Serial.read();
      if (r == 'S' || r == 's') { playerTurn = true;  Serial.println(F("You start.")); break; }
      if (r == 'N' || r == 'n') { playerTurn = false; Serial.println(F("Robot starts.")); break; }
    }
  }
  showBoard();
}

// MAIN LOOP

void loop() {

  // global draw check (just in case)
  if (boardFull()) {
    endMatch("DRAW! The board is full.");
    return;
  }

  if (playerTurn) {
    // PLAYER'S TURN
    // The player physically drops the piece on the real board,
    // then types the column they used (1-7) in the Serial Monitor.

    Serial.println(F("Drop your piece on the board."));
    Serial.println(F("Then type the column you used (1-7):"));

    while (Serial.available() == 0) {}   // wait for input
    int col = Serial.parseInt();

    if (col < 1 || col > 7) {
      Serial.println(F("Invalid column! Type a number between 1 and 7."));
      return;
    }

    if (dropToken(col, player)) {
      Serial.print(F("Player -> column "));
      Serial.println(col);
      showBoard();

      if (checkWin(player)) { endMatch("YOU WON!"); return; }
      if (boardFull())      { endMatch("DRAW!");    return; }
      playerTurn = false;

    } else {
      Serial.println(F("Column full! Choose another one."));
      delay(500);
    }

  } else {
    // ROBOT'S TURN
    // Compute the column -> move the motor -> dispense the piece.

    Serial.println(F("Robot's turn..."));
    delay(500);

    int col = robotLogic();
    if (col == -1) { endMatch("DRAW!"); return; }

    moveToColumn(col);          // move the physical dispenser
    dispense();                 // open the servo and release the piece
    dropToken(col, robot);      // update the virtual board

    Serial.print(F("Robot -> column "));
    Serial.println(col);
    showBoard();

    if (checkWin(robot)) { endMatch("THE MACHINE WON!"); return; }
    if (boardFull())     { endMatch("DRAW!");            return; }

    playerTurn = true;
  }
}
