//Code for calculating wind time variables - Intelligent Jumping Robot

//Pin definitions
const int motorPin1 = 6;  // L293D input pin 2
const int motorPin2 = 5;  // L293D input pin 7
const int buttonPin = 7;  // Pushbutton pin (INPUT_PULLUP)
const int startPin  = 8;
const int relayPin  = 4;


unsigned long motorStartTime = 0;
unsigned long motorRunTime = 0;
unsigned long unwindTime = 0;  // Time to unwind the motor
bool motorRunning = false;
bool lastButtonState = HIGH;  // Last button state (for detecting a press)
bool buttonPressed = false;   // To track whether the button has been pressed
bool start = false;
bool unwinding = false;  // To track if the motor is in the unwinding phase

// Debounce timing variables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 50 ms debounce delay

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Internal pull-up resistor
  pinMode(startPin, INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop() {

  if (digitalRead(startPin) == HIGH) {
    Serial.println("Starting Test...");
    start = true;
  }
  
  while(start){
    if (digitalRead(startPin) == LOW) {
      start = false;
      Serial.println("Resetting Test...");
      digitalWrite(relayPin, LOW);
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      motorStartTime = 0;
      motorRunTime   = 0;
      unwindTime     = 0;
      motorRunning = false;
      unwinding = false;
      lastButtonState = HIGH;
      buttonPressed = false;
      break;
    }
  
    int buttonState = digitalRead(buttonPin); // Read the current state of the button
    digitalWrite(relayPin, HIGH);

    // Check if the button state has changed (button press detected)
    if (buttonState != lastButtonState) {
      lastDebounceTime = millis(); // Record the time of the state change
    }

    // Only proceed if the button state has been stable for the debounce period
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // If the button is pressed (LOW state) and hasn't been processed yet
      if (buttonState == LOW && !buttonPressed) {
        buttonPressed = true; // Mark the button as processed

        if (!motorRunning && !unwinding) {
          // Start the motor (winding)
          digitalWrite(motorPin1, HIGH);  // Rotate in one direction (wind)
          digitalWrite(motorPin2, LOW);
          motorStartTime = millis();  // Record the time when the motor starts
          motorRunning = true;
          Serial.println("Motor started (winding)");
        } else if (motorRunning && !unwinding) {
          // Stop the motor after winding and record the wind time
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);
          motorRunTime = millis() - motorStartTime;  // Calculate the run time
          unwindTime = motorRunTime;  // Set unwind time to equal the wind time
          motorRunning = false;
          
          Serial.print("Motor stopped (winding). Time: ");
          Serial.print(motorRunTime);
          Serial.println(" milliseconds");

          // Start unwinding (reverse motor direction)
          Serial.println("Starting unwind phase...");
          delay(500); // Short delay for visual confirmation in debug output
          digitalWrite(motorPin1, LOW); // Reverse direction for unwinding
          digitalWrite(motorPin2, HIGH); // Reverse direction for unwinding
          motorStartTime = millis(); // Reset start time for unwind
          unwinding = true; // Mark as unwinding
        } else if (unwinding) {
          // Stop the motor after unwinding and record the unwind time
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);
          unwindTime = millis() - motorStartTime;  // Calculate the unwind time
          unwinding = false;
          
          Serial.print("Motor stopped (unwinding). Unwind time: ");
          Serial.print(unwindTime);
          Serial.println(" milliseconds");
        }
      }
    }
    
    // If the button is released, allow the next press to be detected
    if (buttonState == HIGH) {
      buttonPressed = false;
    }

    // Update the last button state
    lastButtonState = buttonState;
  }
}
