// =====================================================================
//
//   CONNECT 4 - MOTOR + SERVO TEST
//   Standalone sketch to test the dispenser hardware only.
//   Type a column (1-7) in the Serial Monitor: the stepper moves the
//   dispenser to that column and the servo drops one piece.
//   Team 7 - UVic-UCC - Projecte Integrat II - 2026
//
//   WARNING: place the dispenser on COLUMN 7 by hand before powering on
//            (that position is taken as the zero reference).
//   TIP: use a dedicated motor power supply with enough current, a weak
//        supply causes brownout resets on the ESP32.
//
//   Libraries: AccelStepper (Mike McCauley) - ESP32Servo (Kevin Harrington)
//
// =====================================================================

#include "soc/soc.h"           // needed to disable the brownout detector
#include "soc/rtc_cntl_reg.h"  // needed to disable the brownout detector
#include <AccelStepper.h>
#include <ESP32Servo.h>

// ----------------------------- PINS ----------------------------------
#define PIN_STEP   17    // A4988 STEP signal
#define PIN_DIR    16    // A4988 DIR signal
#define PIN_SERVO   4    // dispenser servo signal

// ----------------------- MECHANICAL SETTINGS -------------------------
#define STEPS_PER_MM       25    // 200 steps/rev divided by 8 mm lead
#define COLUMN_SPACING_MM  32    // distance in mm between two columns
#define SPEED             800    // steps per second
#define ACCEL             400    // steps per second squared

// --------------------------- SERVO ANGLES ----------------------------
#define SERVO_CLOSED  20    // gate closed (piece is held back)
#define SERVO_OPEN    80    // gate open   (piece drops)
#define SERVO_TIME   600    // ms the gate stays open

// ---------------------------- OBJECTS --------------------------------
AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);
Servo servo;

// =====================================================================
//                          HARDWARE FUNCTIONS
// =====================================================================

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

// =====================================================================
//                               SETUP
// =====================================================================

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // disable the brownout detector
  Serial.begin(115200);

  // motor: current position = column 7 (placed by hand)
  motor.setMaxSpeed(SPEED);
  motor.setAcceleration(ACCEL);
  motor.setCurrentPosition(0);

  // servo: start with the gate closed
  servo.attach(PIN_SERVO);
  servo.write(SERVO_CLOSED);

  Serial.println(F("=== MOTOR + SERVO TEST ==="));
  Serial.println(F("Dispenser must start on column 7."));
  Serial.println(F("Type a column (1-7) and press Enter:"));
}

// =====================================================================
//                             MAIN LOOP
// =====================================================================

void loop() {
  // wait until the user types something in the Serial Monitor
  if (Serial.available() > 0) {
    int col = Serial.parseInt();          // read the requested column

    if (col < 1 || col > 7) {             // reject invalid values
      Serial.println(F("Invalid column! Type a number between 1 and 7."));
      return;
    }

    moveToColumn(col);                    // move the dispenser there
    dispense();                           // drop one piece

    Serial.println(F("Done. Type another column (1-7):"));
  }
}
