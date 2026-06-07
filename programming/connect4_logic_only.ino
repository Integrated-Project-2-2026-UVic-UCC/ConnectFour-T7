// =====================================================================
//
//   CONNECT 4 - LOGIC ONLY (NO HARDWARE)
//   Pure game logic running entirely through the Serial Monitor.
//   No motor, no servo, no sensors. Use this to test the AI and rules.
//   Team 7 - UVic-UCC - Projecte Integrat II - 2026
//
// =====================================================================

// -------------------------- GAME VARIABLES ---------------------------
char board[6][7];               // game board, '-' means empty
const int rows = 6;             // number of rows
const int cols = 7;             // number of columns
const char player = 'X';        // human player token
const char robot  = 'O';        // machine token
bool playerTurn;                // true = player's turn, false = robot's turn

// --------------------- FUNCTION DECLARATIONS -------------------------
void initBoard();
void showBoard();
bool dropToken(int column, char token);
bool checkWin(char token);
int  robotLogic();
bool boardFull();
void endMatch(String message);
bool askRestart();

// =====================================================================
//                           BOARD FUNCTIONS
// =====================================================================

// Fill the whole board with empty cells
void initBoard() {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      board[i][j] = '-';
}

// Print the current board to the Serial Monitor
void showBoard() {
  Serial.println();
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      Serial.print(board[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  for (int j = 1; j <= cols; j++) {   // column numbers under the board
    Serial.print(j);
    Serial.print(" ");
  }
  Serial.println("\n");
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
      if (board[i][j] == token && board[i][j + 1] == token &&
          board[i][j + 2] == token && board[i][j + 3] == token) return true;
  // vertical
  for (int i = 0; i <= rows - 4; i++)
    for (int j = 0; j < cols; j++)
      if (board[i][j] == token && board[i + 1][j] == token &&
          board[i + 2][j] == token && board[i + 3][j] == token) return true;
  // diagonal down-right
  for (int i = 0; i <= rows - 4; i++)
    for (int j = 0; j <= cols - 4; j++)
      if (board[i][j] == token && board[i + 1][j + 1] == token &&
          board[i + 2][j + 2] == token && board[i + 3][j + 3] == token) return true;
  // diagonal down-left
  for (int i = 3; i < rows; i++)
    for (int j = 0; j <= cols - 4; j++)
      if (board[i][j] == token && board[i - 1][j + 1] == token &&
          board[i - 2][j + 2] == token && board[i - 3][j + 3] == token) return true;

  return false;
}

// =====================================================================
//             ROBOT AI (win > block > random available column)
// =====================================================================

int robotLogic() {
  int c;

  // 1. take the winning move if one exists
  for (c = 1; c <= cols; c++) {
    if (board[0][c - 1] == '-') {
      int row = -1;
      for (int i = rows - 1; i >= 0; i--) {
        if (board[i][c - 1] == '-') { row = i; break; }   // lowest free cell
      }
      if (row == -1) continue;

      board[row][c - 1] = robot;          // simulate the move
      if (checkWin(robot)) {
        board[row][c - 1] = '-';          // undo before returning
        return c;
      }
      board[row][c - 1] = '-';            // undo the simulation
    }
  }

  // 2. block the player if they are about to win
  for (c = 1; c <= cols; c++) {
    if (board[0][c - 1] == '-') {
      int row = -1;
      for (int i = rows - 1; i >= 0; i--) {
        if (board[i][c - 1] == '-') { row = i; break; }
      }
      if (row == -1) continue;

      board[row][c - 1] = player;         // simulate player's move
      if (checkWin(player)) {
        board[row][c - 1] = '-';
        return c;
      }
      board[row][c - 1] = '-';
    }
  }

  // 3. otherwise pick a random available column
  int tries = 0;
  do {
    c = random(1, cols + 1);
    tries++;
    if (tries > 100) return -1;           // no column available
  } while (board[0][c - 1] != '-');

  return c;
}

// Return true when the top row is full (board is full)
bool boardFull() {
  for (int j = 0; j < cols; j++)
    if (board[0][j] == '-') return false;
  return true;
}

// =====================================================================
//                            MATCH CONTROL
// =====================================================================

// Handle the end of a match: print the result and either restart or stop
void endMatch(String message) {
  Serial.println(message);

  if (askRestart()) {
    initBoard();
    showBoard();
  } else {
    Serial.println("See you soon!");
    while (true);                         // freeze the program
  }
}

// Ask the player whether they want to play again (S = yes, N = no)
bool askRestart() {
  Serial.println("Play again? (S/N)");
  while (true) {
    if (Serial.available() > 0) {
      char r = Serial.read();
      if (r == 'S' || r == 's') return true;
      if (r == 'N' || r == 'n') return false;
    }
  }
}

// =====================================================================
//                               SETUP
// =====================================================================

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));              // seed the random generator

  initBoard();

  Serial.println("Do you want to start? (S/N)");
  while (true) {
    if (Serial.available() > 0) {
      char r = Serial.read();
      if (r == 'S' || r == 's') { playerTurn = true; break; }
      if (r == 'N' || r == 'n') { playerTurn = false; break; }
    }
  }
  showBoard();
}

// =====================================================================
//                             MAIN LOOP
// =====================================================================

void loop() {

  // stop immediately if the board is full
  if (boardFull()) {
    endMatch("Draw! The board is full.");
    return;
  }

  if (playerTurn) {
    // -------------------- PLAYER'S TURN --------------------
    Serial.println("Choose a column (1-7):");
    while (Serial.available() == 0) {}    // wait for input

    int c = Serial.parseInt();

    if (dropToken(c, player)) {
      showBoard();

      if (checkWin(player))
        endMatch("You won!");
      else
        playerTurn = false;

    } else {
      Serial.println("Column full, try another one.");
    }

  } else {
    // -------------------- ROBOT'S TURN --------------------
    Serial.println("Robot's turn...");

    int c = robotLogic();

    if (c == -1) {
      endMatch("Draw! No columns available.");
      return;
    }

    dropToken(c, robot);

    Serial.print("Robot chooses column: ");
    Serial.println(c);

    showBoard();

    if (checkWin(robot))
      endMatch("The robot won!");
    else
      playerTurn = true;
  }
}
