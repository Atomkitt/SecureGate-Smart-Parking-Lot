#include <Servo.h>
#include <Keypad.h>

const int trigPin = 2;
const int echoPin = 3;
const int servoPin = 11;
const int ledGreenPin = 12;
const int ledRedPin = 13;
const int buzzerPin = A0;
const int dipSwitchPin = A1;
const int segPins[7] = {4, 5, 6, 7, 8, 9, 10};

const byte ROWS = 4; 
const byte COLS = 2; 
char keys[ROWS][COLS] = {
  {'1','2'},
  {'4','5'}, 
  {'7','8'},
  {'*','0'}
};
byte rowPins[ROWS] = {A2, A3, A4, A5}; 
byte colPins[COLS] = {0, 1}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Servo gateServo;
int parkingSpaces = 9; 
int gateState = 0;     
int distanceThreshold = 10; 
bool isExiting = false;

const byte digit_pattern[10][7] = {
  {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1}, 
  {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}  
};

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH); 
  pinMode(dipSwitchPin, INPUT_PULLUP); 
  for(int i=0; i<7; i++) pinMode(segPins[i], OUTPUT);
  gateServo.attach(servoPin);
  gateServo.write(0); 
  digitalWrite(ledRedPin, HIGH);  
  digitalWrite(ledGreenPin, LOW); 
  updateDisplay(parkingSpaces);   
}

void loop() {
  if (digitalRead(dipSwitchPin) == LOW) distanceThreshold = 20; 
  else distanceThreshold = 10; 

  long distance = getFilteredDistance();
  
  char key = keypad.getKey();
  if (key) {
    if (key == '1') { 
      isExiting = false;
      gateState = 1;
    }
    if (key == '4') { 
      isExiting = true;
      gateState = 1;
    }
    if (key == '2') {
      if (parkingSpaces < 9) parkingSpaces++;
      updateDisplay(parkingSpaces);
    }
    if (key == '5') {
      if (parkingSpaces > 0) parkingSpaces--;
      updateDisplay(parkingSpaces);
    }
    if (key == '0') {              
      parkingSpaces = 9;
      updateDisplay(parkingSpaces);
      beepBuzzer(2);
    }
  }

  switch (gateState) {
    case 0: 
      if (distance > 0 && distance <= distanceThreshold && parkingSpaces > 0) {
        isExiting = false;
        gateState = 1; 
      }
      break;
    case 1: 
      digitalWrite(ledRedPin, LOW);
      digitalWrite(ledGreenPin, HIGH); 
      gateServo.write(90);             
      beepBuzzer(1);                   
      gateState = 2;                   
      delay(500);
      break;
    case 2: 
      if (distance > distanceThreshold + 5) {
        delay(1500); 
        if (isExiting) {
          if (parkingSpaces < 9) parkingSpaces++;
        } else {
          if (parkingSpaces > 0) parkingSpaces--;
        }
        updateDisplay(parkingSpaces);
        gateState = 3;                 
      }
      break;
    case 3: 
      gateServo.write(0);              
      digitalWrite(ledGreenPin, LOW);
      digitalWrite(ledRedPin, HIGH);   
      beepBuzzer(1);                   
      gateState = 0;                   
      delay(500);
      break;
  }
}

long getFilteredDistance() {
  long d1 = readUltrasonic(); delay(10);
  long d2 = readUltrasonic(); delay(10);
  long d3 = readUltrasonic();
  if (d1 > d2) { long t = d1; d1 = d2; d2 = t; }
  if (d2 > d3) { long t = d2; d2 = d3; d3 = t; }
  if (d1 > d2) { long t = d1; d1 = d2; d2 = t; }
  return d2; 
}

long readUltrasonic() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); 
  if (duration == 0) return 999; 
  return duration * 0.034 / 2;
}

void beepBuzzer(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzerPin, LOW); delay(100);
    digitalWrite(buzzerPin, HIGH); delay(100);
  }
}

void updateDisplay(int num) {
  if (num < 0 || num > 9) return;
  for (int i = 0; i < 7; i++) digitalWrite(segPins[i], digit_pattern[num][i]);
}