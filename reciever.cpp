// This code is to be uploaded onto the robot (receiver end)

// Robot control logic - Intelligent Jumping Robot

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pin Definitions for nRF24L01
#define CE_PIN 7
#define CSN_PIN 8

// Pin assignment for Receiver Arduino
#define relayPin    4    // Digital output for electromagnet
#define motorFwd    6    // Motor forward
#define motorBwd    5    // Motor backward
#define PWM_L   10   // PWM Enable pin for Motor 2
#define PWM_R   11   // PWM Enable pin for Motor 1
#define DIR1_R  13   // Input 1 for Motor 1
#define DIR2_R  9    // Input 2 for Motor 1
#define DIR1_L  3    // Input 1 for Motor 2
#define DIR2_L  2    // Input 2 for Motor 2

// Create RF24 object
RF24 radio(CE_PIN, CSN_PIN);

// Address of the pipe to communicate
const byte address[6] = "00001";

// State machine
enum ActState {
  IDLE, WINDING, SHUNTING, UNWINDING, PRIMED
} actState = IDLE;

unsigned long stageStartTime = 0; // Tracks when a stage starts
const unsigned long windTime = 24400;   // Time for motor to wind
const unsigned long shuntTime = 1900;   // Time for shunt
const unsigned long unwindTime = 24200; // Time for motor to unwind

bool primedForRelease = false; // Flag to enable latch release

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize nRF24L01
  radio.begin();
  radio.openReadingPipe(0, address); // Set the address of the transmitter
  radio.setPALevel(RF24_PA_MAX);     // Set power level
  radio.startListening();            // Set as receiver

  // Initialize motor pins
  pinMode(PWM_L, OUTPUT);
  pinMode(DIR1_L, OUTPUT);
  pinMode(DIR2_L, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(DIR1_R, OUTPUT);
  pinMode(DIR2_R, OUTPUT);

  // Initialize Latch pins
  pinMode(relayPin, OUTPUT);
  pinMode(motorFwd, OUTPUT);
  pinMode(motorBwd, OUTPUT);
}

void loop() {
  // Check if data is available
  if (radio.available()) {
    int data[4];
    radio.read(&data, sizeof(data)); // Read data from transmitter

    int pot1Value = data[0]; // Potentiometer 1 value
    int pot2Value = data[1]; // Potentiometer 2 value

    bool startState = data[2]; // Start button reading
    bool releaseState = data[3]; // Release button reading

    // Process received data
    if (startState && actState == IDLE && !primedForRelease) {
      Serial.println("Starting Actuation Sequence...");
      actState = WINDING;
      stageStartTime = millis();
    }

    if (releaseState && primedForRelease) {
      // Release latch
      Serial.println("BOING!");
      digitalWrite(relayPin, LOW);
      primedForRelease = false;
    }

        // Actuation sequence based on the state machine
    switch (actState) {
      case WINDING:
        stage1();
        if (millis() - stageStartTime >= windTime) {
          actState = SHUNTING;
          stageStartTime = millis();
        }
        break;

      case SHUNTING:
        stage2();
        if (millis() - stageStartTime >= shuntTime) {
          actState = UNWINDING;
          stageStartTime = millis();
        }
        break;

      case UNWINDING:
        stage3();
        if (millis() - stageStartTime >= unwindTime) {
          actState = PRIMED;
          primedForRelease = true;
          Serial.println("System Primed for Release.");
        }
        break;

      case PRIMED:
        // Wait for release button press
        Serial.println("Waiting");
        stage4();
        actState = IDLE;
        break;

      case IDLE:
      default:
        differentialdrive(pot1Value, pot2Value, startState, releaseState); // Pass potentiometer values and button states to the differential drive function
        break;
    }
  }
}

void stage1() {
  // Motor winds, electromagnet OFF
  digitalWrite(motorFwd, HIGH);
  digitalWrite(motorBwd, LOW);
}

void stage2() {
  // Motor shunts, then stops. Electromagnet ON
  digitalWrite(motorFwd, LOW);
  digitalWrite(motorBwd, LOW);
  digitalWrite(relayPin, HIGH);
}

void stage3() {
  // Motor unwinds, electromagnet ON
  digitalWrite(motorFwd, LOW);
  digitalWrite(motorBwd, HIGH);
  digitalWrite(relayPin, HIGH);
}

void stage4() {
  // Await launch. Electromagnet ON
  digitalWrite(motorFwd, LOW);
  digitalWrite(motorBwd, LOW);
  digitalWrite(relayPin, HIGH);
}

void differentialdrive(int pot1Value, int pot2Value, bool startState, bool releaseState) {
  // Exit logic: Stop differential drive if the start or release button is pressed
  if ((startState && !primedForRelease && actState == IDLE) || 
      (releaseState && primedForRelease && actState == IDLE)) {
    // Motor 1 stopped
    analogWrite(PWM_R, 0); // Stop motor
    digitalWrite(DIR1_R, LOW);
    digitalWrite(DIR2_R, LOW);
    // Motor 2 stopped
    analogWrite(PWM_L, 0); // Stop motor
    digitalWrite(DIR1_L, LOW);
    digitalWrite(DIR2_L, LOW);  

    Serial.println("Differential Drive Exited.");
    return; // Exit differential drive
  }

  // Map Potentiometer values to motor control
  int throttle = map(pot1Value, 0, 1023, -255, 255);
  int turn = map(pot2Value, 0, 1023, -255, 255);

  // Compute motor speeds
  int leftSpeed  = throttle + turn;
  int rightSpeed = throttle - turn;

  // Clamp to valid PWM range
  leftSpeed  = constrain(leftSpeed,  -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  setMotor(PWM_L, DIR1_L, DIR2_L, leftSpeed);
  setMotor(PWM_R, DIR1_R, DIR2_R, rightSpeed);

  delay(10); // small delay to smooth control
}

void setMotor (int pwmPin, int dir1, int dir2, int speed) {
  if (speed > 0) {
    digitalWrite(dir1, HIGH);
    digitalWrite(dir2, LOW);
  } else if (speed < 0) {
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, HIGH);
    speed = -speed;
  } else {
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, LOW);
  }

  analogWrite(pwmPin, speed);
}

