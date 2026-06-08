// CONNECT 4 - SENSOR TEST
// Standalone sketch to check the 7 column photocells.
// Each sensor is ACTIVE LOW: it reads 0 (LOW) when a piece blocks it
// and 1 (HIGH) when the beam is clear.
// Open the Serial Monitor at 115200 baud and watch the readings.
// Team 7 - UVic-UCC - Projecte Integrat II - 2026
// NOTE: GPIO35 is input-only and has NO internal pull-up, so column 7
// needs an EXTERNAL pull-up resistor (about 10k to 3V3).

// PINS
// One photocell per column. Index 0 = column 1 ... index 6 = column 7.
#define NUM_SENSORS 7
int sensors[NUM_SENSORS] = {13, 12, 14, 27, 26, 25, 33};

// SETUP

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Photocell test - they detect on LOW");

  // configure every sensor pin with the internal pull-up
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensors[i], INPUT_PULLUP);
  }
}

// MAIN LOOP

void loop() {
  // print the state of every sensor on a single line
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("C");
    Serial.print(i + 1);                 // human-friendly column number
    Serial.print(" GPIO");
    Serial.print(sensors[i]);            // physical pin
    Serial.print(" = ");
    Serial.print(digitalRead(sensors[i]));  // 0 = blocked, 1 = clear
    Serial.print("   ");
  }
  Serial.println();
  delay(300);                            // refresh rate
}
