/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/AnalogReadSerial/
*/
const int buttonPin = 2;
const int RledPin = 9;
const int GledPin = 10;
const int BledPin = 11;

int mood = 0;
const int neutralMood = 10;
int buttonState = 0;
bool ButtonPressed = false;
unsigned long touchedTimer = 0;
unsigned long reducedTimer = 0;
const long unTouchInterval = 5000;
const long reducedInterval = 5000;
unsigned long previousMillis = 0;
int fadingDirection = 1;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(buttonPin, INPUT);
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  mood = neutralMood;
}

// the loop routine runs over and over again forever:
void loop(){
  showLEDState(mood);
  buttonState = digitalRead(buttonPin);

  if(buttonState == HIGH && !ButtonPressed){
    mood = mood + 1;
    if(mood > 20) mood = 20;

    touchedTimer = millis();
    ButtonPressed = true;
  }
  if(buttonState == LOW && ButtonPressed){
    ButtonPressed = false;
  } 

  unsigned long currentTimer = millis();
  if(currentTimer - touchedTimer > unTouchInterval){
    if(currentTimer - reducedTimer > reducedInterval){
      mood = mood - 1;
      if(mood < 0) mood = 0;
      reducedTimer = currentTimer;
    }
  }
}
void showLEDState(int state) {
  float brightnessInterval = 255 / 10.0;
  if(mood >= neutralMood){
    analogWrite(RledPin, 255);
    analogWrite(GledPin, brightnessInterval * (mood - neutralMood));
    analogWrite(BledPin, 255 - brightnessInterval * (neutralMood - mood));
  }
  else{
    analogWrite(RledPin, 255 - brightnessInterval * (neutralMood - mood));
    analogWrite(GledPin, brightnessInterval * (neutralMood - mood));
    analogWrite(BledPin, 255);
  }

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > 1000){
    mood = mood + fadingDirection;
    if(mood >= 20 || mood <= 0)
      fadingDirection = -fadingDirection;
    previousMillis = currentMillis;
  }
}
