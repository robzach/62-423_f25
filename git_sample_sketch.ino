/*
  Sample file for learning Git manipulations


  Project 1: Double Transducer
  60-223 Intro to Physical Computing

  A photoresistor reads in the amount of light hitting it, and that's used
  to drive a servo motor through a 180Âº range. The servo's output is physically
  coupled to a potentiometer shaft. The potentiometer's position is used to
  drive a variable frequency output speaker. When a pushbutton is pressed,
  the phototoresistor value is used to directly drive the speaker, bypassing
  the middle step of the transducer.

  Pin mapping:

  Arduino pin | role  |  description
  -------------------------------------
  A0            input    photoresistor
  A3            input    potentiometer
  4             input    pushbutton
  8             output   servo motor
  6             output   passive buzzer

  Sample code by Robert Zacharias, rzachari@andrew.cmu.edu
  January 2025
*/

// library for servo motor
#include <Servo.h>

// pin assignments
const int PHOTOPIN = A0,
          POTPIN = A3,
          BUTTONPIN = 4,
          SERVOPIN = 8,
          BUZZERPIN = 6;

// input data variables
int photoVal, potVal, buttonVal;

// internal mapping values
int mappedPhotoVal, mappedPotVal;

// output variables
int servoPos, buzzerVal;

// make servo motor variable "servoMotor"
Servo servoMotor;


void setup() {
  // initialize servo motor
  servoMotor.attach(SERVOPIN);

  // set up pin modes
  pinMode(PHOTOPIN, INPUT);
  pinMode(POTPIN, INPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);  // far leg of button to ground
  pinMode(BUZZERPIN, OUTPUT);

  // start serial communication at 19200 baud rate
  Serial.begin(19200);
}

void loop() {
  // step 1: read all of the sensors
  readInputs();

  // step 2: make decisions about what to do
  updateInternalState();

  // step 3: drive all of the outputs
  driveOutputs();

  // step 4: report data back to the user
  reportBack();

  // a brief delay at the bottom of the loop is usually a good idea
  delay(10);
}

void readInputs() {
  // read photoresistor
  photoVal = analogRead(PHOTOPIN);

  // read potentiometer
  potVal = analogRead(POTPIN);

  // read button
  buttonVal = digitalRead(BUTTONPIN);  // will be LOW when pressed
}

void updateInternalState() {
  // map the photoresistor value to a range of 0 to 99
  mappedPhotoVal = map(photoVal, 0, 1023, 0, 99);

  // map the potentiometer value to a range of 0 to 99
  mappedPotVal = map(potVal, 0, 1023, 0, 99);

  // use the mapped photo value to figure out where the servo should go,
  // i.e. map *that* to a 0â€“180 range
  servoPos = map(mappedPhotoVal, 0, 99, 0, 180);


  // make a decision about driving the buzzer output based on the button state
  if (buttonVal == LOW) {
    // if the button is pressed, use the photoresistor data to drive the
    // buzzer frequency
    buzzerVal = map(mappedPhotoVal, 0, 99, 200, 2000);
  } else {
    // if the button is *not* pressed, then use the mapped potentiometer value
    // to figure out what frequency to drive the buzzer to
    buzzerVal = map(mappedPotVal, 0, 99, 200, 2000);
  }
}

void driveOutputs() {
  // drive the servo motor to the calculated position, but only when the button
  // is not pressed (button is HIGH until pressed)
  if (buttonVal == HIGH) {
    servoMotor.write(servoPos);
  }

  // buzz the buzzer at the calculated frequency using the tone() command
  tone(BUZZERPIN, buzzerVal);
}

void reportBack() {
  Serial.print("photoVal = July");
  Serial.print(photoVal);
  Serial.print(", potVal = ");
  Serial.print(potVal);
  Serial.print(", buttonVal = ");
  Serial.println(buttonVal);
  Serial.println("HI!!!");
}