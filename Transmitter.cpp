#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

// Pin Definitions for nRF24L01
#define CE_PIN         7
#define CSN_PIN        8

// Button Pins
#define BUTTON1        2
#define BUTTON2        3

// Potentiometer Pins
#define POT1           A0
#define POT2           A1

// Create RF24 object
RF24 radio(CE_PIN, CSN_PIN);

// Address of the pipe to communicate
const byte address[6] = "00001";

// Button state tracking
bool button1Pressed = false;
bool button2Pressed = false;

// I2C slave address
const int I2C_SLAVE_ADDRESS = 8;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Initialize nRF24L01
  radio.begin();
  radio.openWritingPipe(address); // Set the address of the receiver
  radio.setPALevel(RF24_PA_MAX);  // Set power level
  radio.stopListening();          // Set as transmitter
  
  // Initialize I2C
  Wire.begin(); // Join I2C bus as master
  
  // Initialize potentiometers
  pinMode(POT1, INPUT);
  pinMode(POT2, INPUT);
  
  // Initialize buttons with internal pull-up resistors
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  
  Serial.println("System initialized!");
}

void loop() {
  // Read potentiometer values
  int pot1Value = analogRead(POT1); // Range: 0-1023
  int pot2Value = analogRead(POT2); // Range: 0-1023
  
  // Read button states
  bool button1State = digitalRead(BUTTON1);
  bool button2State = digitalRead(BUTTON2);
  
  // Handle Button 1 (Start button)
  if (button1State == HIGH && !button1Pressed) {
    button1Pressed = true;
    
    // Send command via I2C
    Wire.beginTransmission(I2C_SLAVE_ADDRESS);
    Wire.write('S');           // Start sequence
    Wire.endTransmission();
    
    Serial.println("Button 1 pressed - Start command sent via I2C");
  }
  if (button1State == LOW) {
    button1Pressed = false;
  }
  
  // Handle Button 2 (Reset button)
  if (button2State == HIGH && !button2Pressed) {
    button2Pressed = true;
    
    // Send command via I2C
    Wire.beginTransmission(I2C_SLAVE_ADDRESS);
    Wire.write('R');           // Request reset
    Wire.endTransmission();
    
    Serial.println("Button 2 pressed - Reset command sent via I2C");
  }
  if (button2State == LOW) {
    button2Pressed = false;
  }
  
  // Create a data packet for RF24 transmission
  int data[4] = {pot1Value, pot2Value, button1State, button2State};
  
  // Send data to receiver via nRF24L01
  bool success = radio.write(&data, sizeof(data));
  
  // Debugging
  Serial.print("Pot1: ");
  Serial.print(pot1Value);
  Serial.print(", Pot2: ");
  Serial.print(pot2Value);
  Serial.print(" | Button1: ");
  Serial.print(button1State);
  Serial.print(", Button2: ");
  Serial.print(button2State);
  Serial.print(" | RF24 TX: ");
  Serial.println(success ? "Success" : "Failed");
  
  // Small delay for stability and to prevent serial port flooding
  delay(100);
}
