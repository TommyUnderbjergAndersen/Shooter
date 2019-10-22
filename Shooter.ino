#include <Ticker.h>
#include "pitches.h"

const int buttonPin = 5;     // the number of the pushbutton pin
bool buttonState = true;         // variable for reading the pushbutton status
bool prevButtonState = true;

const int buzzerPin = 15;

const int ledPinR = 14;
const int ledPinG = 12;
const int ledPinB = 13;

float curSeconds = 10.0;
bool hit = false;
bool curTargetOn = false;
unsigned int score = 0;

bool endGame = false;

Ticker t;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(buttonPin), hitInterrupt, FALLING);
  
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(115200);
  Serial.println("Start...");
  targetOff();
}

void loop() {
  //TODO: replace by interrups
  buttonState = digitalRead(buttonPin);
  if (!buttonState && prevButtonState) {
    hit = true;
  }
  prevButtonState = buttonState;
  
  //React to hit
  if (hit) {
    hit = false;
    if (curTargetOn){
      endGame = false;
      score++;
      curTargetOn = false;
      ledBlue();
      soundHit();
    }    
  }
}

void hitInterrupt() {
  hit = true;
}


void targetOn() {
  Serial.print("ON: ");
  Serial.println(curSeconds);
  curTargetOn = true;
  ledGreen();
  curSeconds = curSeconds - 0.1;
  endGame = true;
  t.once(curSeconds, targetOff);
  soundShoot();  
}  

void targetOff() {
  Serial.println("OFF");
  curTargetOn = false;
  ledRed();

  if (endGame) {
    Serial.print("Final SCORE: ");
    Serial.println(score);
    Serial.println("NEW GAME");
    score = 0;
    curSeconds = 10;
  }

  if(curSeconds <= 0.0){
    Serial.println("***MAX POINTS***");
    Serial.println("NEW GAME");
    curSeconds = 10.0;
  }
  
  t.once(5,targetOn);
}   

void ledRed(){
    digitalWrite(ledPinR, HIGH);
    digitalWrite(ledPinG, LOW);
    digitalWrite(ledPinB, LOW);
}

void ledGreen(){
    digitalWrite(ledPinR, LOW);
    digitalWrite(ledPinG, HIGH);
    digitalWrite(ledPinB, LOW);
    play(NOTE_E5);
}

void ledBlue(){
    digitalWrite(ledPinR, LOW);
    digitalWrite(ledPinG, LOW);
    digitalWrite(ledPinB, HIGH);
}

void soundHit(){
    play(NOTE_E5);
    play(NOTE_E5);
    play(NOTE_A5);
}

void soundShoot(){
    play(NOTE_A4);
    play(NOTE_A4);
}

void play(unsigned int freq) {
  analogWriteFreq(freq);
  analogWrite(buzzerPin, 512);
  delay(250);
  analogWrite(buzzerPin, 0);
}
