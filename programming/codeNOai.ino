char taula[6][7];
const int fila_max = 6;
const int col_max = 7;
const char jugador = 'X';
const char robot = 'O';
bool tornJugador;

void inicialitzaTaula();
void mostraTaula();
bool tiraFitxa(int columna, char fitxa);
bool comprovaGuany(char fitxa);
int columnaRobotLogica();
bool taulaPlena();
void finalPartida(String missatge);
bool preguntaReinici();

void inicialitzaTaula() {
  for (int i = 0; i < fila_max; i++)
    for (int j = 0; j < col_max; j++)
      taula[i][j] = '-';
}

void mostraTaula() {
  Serial.println();
  for (int i = 0; i < fila_max; i++) {
    for (int j = 0; j < col_max; j++) {
      Serial.print(taula[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  for (int j = 1; j <= col_max; j++) {
    Serial.print(j);
    Serial.print(" ");
  }
  Serial.println("\n");
}

bool tiraFitxa(int columna, char fitxa) {
  columna--;
  if (columna < 0 || columna >= col_max) return false;

  for (int i = fila_max - 1; i >= 0; i--) {
    if (taula[i][columna] == '-') {
      taula[i][columna] = fitxa;
      return true;
    }
  }
  return false;
}

bool comprovaGuany(char fitxa) {
  for (int i = 0; i < fila_max; i++)
    for (int j = 0; j <= col_max - 4; j++)
      if (taula[i][j] == fitxa && taula[i][j + 1] == fitxa &&
          taula[i][j + 2] == fitxa && taula[i][j + 3] == fitxa) return true;

  for (int i = 0; i <= fila_max - 4; i++)
    for (int j = 0; j < col_max; j++)
      if (taula[i][j] == fitxa && taula[i + 1][j] == fitxa &&
          taula[i + 2][j] == fitxa && taula[i + 3][j] == fitxa) return true;

  for (int i = 0; i <= fila_max - 4; i++)
    for (int j = 0; j <= col_max - 4; j++)
      if (taula[i][j] == fitxa && taula[i + 1][j + 1] == fitxa &&
          taula[i + 2][j + 2] == fitxa && taula[i + 3][j + 3] == fitxa) return true;

  for (int i = 3; i < fila_max; i++)
    for (int j = 0; j <= col_max - 4; j++)
      if (taula[i][j] == fitxa && taula[i - 1][j + 1] == fitxa &&
          taula[i - 2][j + 2] == fitxa && taula[i - 3][j + 3] == fitxa) return true;

  return false;
}

int columnaRobotLogica() {
  int c;

  // 1. Guanyar
  for (c = 1; c <= col_max; c++) {
    if (taula[0][c - 1] == '-') {
      int fila = -1;
      for (int i = fila_max - 1; i >= 0; i--) {
        if (taula[i][c - 1] == '-') { fila = i; break; }
      }
      if (fila == -1) continue;

      taula[fila][c - 1] = robot;
      if (comprovaGuany(robot)) {
        taula[fila][c - 1] = '-';
        return c;
      }
      taula[fila][c - 1] = '-';
    }
  }

  // 2. Bloquejar
  for (c = 1; c <= col_max; c++) {
    if (taula[0][c - 1] == '-') {
      int fila = -1;
      for (int i = fila_max - 1; i >= 0; i--) {
        if (taula[i][c - 1] == '-') { fila = i; break; }
      }
      if (fila == -1) continue;

      taula[fila][c - 1] = jugador;
      if (comprovaGuany(jugador)) {
        taula[fila][c - 1] = '-';
        return c;
      }
      taula[fila][c - 1] = '-';
    }
  }

  // 3. Aleatori (CORREGIT)
  int intents = 0;
  do {
    c = random(1, col_max + 1);
    intents++;
    if (intents > 100) return -1;
  } while (taula[0][c - 1] != '-'); // NOMÉS això!

  return c;
}

bool taulaPlena() {
  for (int j = 0; j < col_max; j++)
    if (taula[0][j] == '-') return false;
  return true;
}

void finalPartida(String missatge) {
  Serial.println(missatge);

  if (preguntaReinici()) {
    inicialitzaTaula();
    mostraTaula();
  } else {
    Serial.println("Fins aviat!");
    while (true);
  }
}

bool preguntaReinici() {
  Serial.println("Vols tornar a jugar? (S/N)");
  while (true) {
    if (Serial.available() > 0) {
      char r = Serial.read();
      if (r == 'S' || r == 's') return true;
      if (r == 'N' || r == 'n') return false;
    }
  }
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  inicialitzaTaula();

  Serial.println("Vols començar tu? (S/N)");
  while (true) {
    if (Serial.available() > 0) {
      char r = Serial.read();
      if (r == 'S' || r == 's') { tornJugador = true; break; }
      if (r == 'N' || r == 'n') { tornJugador = false; break; }
    }
  }
  mostraTaula();
}

void loop() {

  //  SOLUCIÓ DEFINITIVA AL BUCLE
  if (taulaPlena()) {
    finalPartida("Empat! La taula està plena.");
    return;
  }

  if (tornJugador) {
    Serial.println("Tria la columna (1-7):");
    while (Serial.available() == 0) {}

    int c = Serial.parseInt();

    if (tiraFitxa(c, jugador)) {
      mostraTaula();

      if (comprovaGuany(jugador))
        finalPartida("Has guanyat!");
      else
        tornJugador = false;

    } else {
      Serial.println("Columna plena, prova una altra.");
    }

  } else {
    Serial.println("Torn del robot...");

    int c = columnaRobotLogica();

    if (c == -1) {
      finalPartida("Empat! No hi ha columnes disponibles.");
      return;
    }

    tiraFitxa(c, robot);

    Serial.print("Robot tria columna: ");
    Serial.println(c);

    mostraTaula();

    if (comprovaGuany(robot))
      finalPartida("El robot ha guanyat!");
    else
      tornJugador = true;
  }
}
