/*
 * ============================================================
 *  Connect 4 — Lògica del joc (versió corregida)
 *  ESP32 · Arduino Framework
 * ============================================================
 *  CANVIS RESPECTE LA VERSIÓ ORIGINAL:
 *
 *  [FIX 1] while(true) → esp_deep_sleep_start()
 *          El watchdog de l'ESP32 reiniciava la placa als ~5s.
 *
 *  [FIX 2] tornJugador no es reiniciava entre partides.
 *          Ara finalPartida() torna a preguntar qui comença.
 *
 *  [FIX 3] randomSeed(analogRead(0)) → randomSeed(esp_random())
 *          GPIO0 és un pin d'strapping, no fiable com a ADC.
 *          esp_random() usa el generador hardware de l'ESP32.
 *
 *  [FIX 4] Serial.parseInt() tenia timeout d'1s i retornava 0.
 *          Ara es llegeix caràcter a caràcter, com la resta del codi.
 *
 *  [FIX 5] String → const char* + macro F()
 *          Evita fragmentació de la heap en sistemes embeguts.
 *
 *  [FIX 6] taulaPlena() es comprova ara just després de cada tirada,
 *          no al principi del loop (que necessitava una iteració extra).
 * ============================================================
 */

#include "esp_sleep.h"   // [FIX 1] Necessari per esp_deep_sleep_start()

// ── Tauler ────────────────────────────────────────────────────
char taula[6][7];
const int fila_max = 6;
const int col_max  = 7;

const char jugador = 'X';
const char robot   = 'O';

bool tornJugador;

// ── Declaració de funcions ────────────────────────────────────
void inicialitzaTaula();
void mostraTaula();
bool tiraFitxa(int columna, char fitxa);
bool comprovaGuany(char fitxa);
int  columnaRobotLogica();
bool taulaPlena();
void finalPartida(const char* missatge);   // [FIX 5] const char* en lloc de String
bool preguntaReinici();
void preguntaQuiComenca();                 // [FIX 2] Nova funció separada
int  llegeixColumna();                     // [FIX 4] Nova funció de lectura Serial

// ─────────────────────────────────────────────────────────────

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
      Serial.print(' ');
    }
    Serial.println();
  }
  for (int j = 1; j <= col_max; j++) {
    Serial.print(j);
    Serial.print(' ');
  }
  Serial.println(F("\n"));
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
  return false; // Columna plena
}

bool comprovaGuany(char fitxa) {
  // Horitzontal
  for (int i = 0; i < fila_max; i++)
    for (int j = 0; j <= col_max - 4; j++)
      if (taula[i][j]   == fitxa && taula[i][j+1] == fitxa &&
          taula[i][j+2] == fitxa && taula[i][j+3] == fitxa) return true;

  // Vertical
  for (int i = 0; i <= fila_max - 4; i++)
    for (int j = 0; j < col_max; j++)
      if (taula[i][j]   == fitxa && taula[i+1][j] == fitxa &&
          taula[i+2][j] == fitxa && taula[i+3][j] == fitxa) return true;

  // Diagonal ↘
  for (int i = 0; i <= fila_max - 4; i++)
    for (int j = 0; j <= col_max - 4; j++)
      if (taula[i][j]     == fitxa && taula[i+1][j+1] == fitxa &&
          taula[i+2][j+2] == fitxa && taula[i+3][j+3] == fitxa) return true;

  // Diagonal ↙
  for (int i = 3; i < fila_max; i++)
    for (int j = 0; j <= col_max - 4; j++)
      if (taula[i][j]     == fitxa && taula[i-1][j+1] == fitxa &&
          taula[i-2][j+2] == fitxa && taula[i-3][j+3] == fitxa) return true;

  return false;
}

int columnaRobotLogica() {
  int c, fila;

  // 1. Guanyar si és possible
  for (c = 1; c <= col_max; c++) {
    if (taula[0][c - 1] != '-') continue; // Columna plena, saltar
    fila = -1;
    for (int i = fila_max - 1; i >= 0; i--)
      if (taula[i][c - 1] == '-') { fila = i; break; }
    if (fila == -1) continue;

    taula[fila][c - 1] = robot;
    bool guanya = comprovaGuany(robot);
    taula[fila][c - 1] = '-';
    if (guanya) return c;
  }

  // 2. Bloquejar el jugador
  for (c = 1; c <= col_max; c++) {
    if (taula[0][c - 1] != '-') continue;
    fila = -1;
    for (int i = fila_max - 1; i >= 0; i--)
      if (taula[i][c - 1] == '-') { fila = i; break; }
    if (fila == -1) continue;

    taula[fila][c - 1] = jugador;
    bool guanyaria = comprovaGuany(jugador);
    taula[fila][c - 1] = '-';
    if (guanyaria) return c;
  }

  // 3. Moviment aleatori en una columna disponible
  int intents = 0;
  do {
    c = random(1, col_max + 1);
    intents++;
    if (intents > 100) return -1; // Salvaguarda (taulell gairebé ple)
  } while (taula[0][c - 1] != '-');

  return c;
}

bool taulaPlena() {
  for (int j = 0; j < col_max; j++)
    if (taula[0][j] == '-') return false;
  return true;
}

// [FIX 4] Lectura de columna caràcter a caràcter (sense timeout)
int llegeixColumna() {
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c >= '1' && c <= '7') return c - '0'; // Converteix char a int
      // Ignorar salts de línia, espais i caràcters invàlids
    }
  }
}

// [FIX 2] Funció separada per preguntar qui comença (reutilitzable)
void preguntaQuiComenca() {
  Serial.println(F("Vols començar tu? (S/N)"));
  while (true) {
    if (Serial.available() > 0) {
      char r = Serial.read();
      if (r == 'S' || r == 's') { tornJugador = true;  return; }
      if (r == 'N' || r == 'n') { tornJugador = false; return; }
    }
  }
}

bool preguntaReinici() {
  Serial.println(F("Vols tornar a jugar? (S/N)"));
  while (true) {
    if (Serial.available() > 0) {
      char r = Serial.read();
      if (r == 'S' || r == 's') return true;
      if (r == 'N' || r == 'n') return false;
    }
  }
}

// [FIX 5] const char* en lloc de String
// [FIX 2] Ara torna a preguntar qui comença si es reinicia
void finalPartida(const char* missatge) {
  Serial.println(missatge);

  if (preguntaReinici()) {
    inicialitzaTaula();
    preguntaQuiComenca();  // [FIX 2] Reinicia el torn correctament
    mostraTaula();
  } else {
    Serial.println(F("Fins aviat!"));
    esp_deep_sleep_start();  // [FIX 1] En lloc de while(true)
  }
}

// ─────────────────────────────────────────────────────────────
//  setup()
// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  randomSeed(esp_random());  // [FIX 3] Generador hardware de l'ESP32

  inicialitzaTaula();
  preguntaQuiComenca();      // [FIX 2] Reutilitza la funció nova
  mostraTaula();
}

// ─────────────────────────────────────────────────────────────
//  loop()
// ─────────────────────────────────────────────────────────────
void loop() {

  if (tornJugador) {
    // ── Torn del jugador ──────────────────────────────────────
    Serial.println(F("Tria la columna (1-7):"));

    int c = llegeixColumna();  // [FIX 4]

    if (tiraFitxa(c, jugador)) {
      mostraTaula();

      if (comprovaGuany(jugador)) {
        finalPartida("Has guanyat! Enhorabona!");  // [FIX 5]
        return;
      }

      // [FIX 6] Comprovació d'empat just després de la tirada
      if (taulaPlena()) {
        finalPartida("Empat! La taula esta plena.");
        return;
      }

      tornJugador = false;

    } else {
      Serial.println(F("Columna plena o invalida, prova una altra."));
    }

  } else {
    // ── Torn del robot ────────────────────────────────────────
    Serial.println(F("Torn del robot..."));

    int c = columnaRobotLogica();

    if (c == -1) {
      finalPartida("Empat! No hi ha columnes disponibles.");
      return;
    }

    tiraFitxa(c, robot);

    Serial.print(F("Robot tria columna: "));
    Serial.println(c);

    mostraTaula();

    if (comprovaGuany(robot)) {
      finalPartida("El robot ha guanyat!");  // [FIX 5]
      return;
    }

    // [FIX 6] Comprovació d'empat just després de la tirada del robot
    if (taulaPlena()) {
      finalPartida("Empat! La taula esta plena.");
      return;
    }

    tornJugador = true;
  }
}
