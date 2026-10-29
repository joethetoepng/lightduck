
const int buttonPin = 2;
const int RledPin = 3;
const int GledPin = 5;
const int BledPin = 6;

int mood = 0;
float smoothMood = 0.0;

bool ButtonPressed = false;

unsigned long touchedTimer = 0;
unsigned long reducedTimer = 0;
unsigned long fadeTimer = 0;

const unsigned long unTouchInterval = 2000;
const unsigned long reducedInterval = 100;
const unsigned long fadeInterval = 10;

const float fadeSpeed = 0.3;

void setup() {
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  // Button between pin 2 and GND
  pinMode(buttonPin, INPUT_PULLUP);

  showLEDState(smoothMood);
}

void loop() {

  unsigned long currentMillis = millis();
  int buttonState = digitalRead(buttonPin);

  // PRESS: MOVE TOWARD RED
  if (buttonState == LOW && !ButtonPressed) {
    mood++;

    if (mood > 20)
      mood = 20;

    touchedTimer = currentMillis;
    reducedTimer = currentMillis;
    ButtonPressed = true;
  }

  // RELEASE BUTTON
  if (buttonState == HIGH && ButtonPressed) {
    ButtonPressed = false;
  }

  // INACTIVITY: MOVE BACK TO BLUE
  if (currentMillis - touchedTimer >= unTouchInterval) {
    if (currentMillis - reducedTimer >= reducedInterval) {
      mood--;

      if (mood < 0)
        mood = 0;

      reducedTimer = currentMillis;
    }
  }

  // SMOOTH COLOR TRANSITION
  if (currentMillis - fadeTimer >= fadeInterval) {
    fadeTimer = currentMillis;

    if (smoothMood < mood) {
      smoothMood += fadeSpeed;
      if (smoothMood > mood)
        smoothMood = mood;
    }
    else if (smoothMood > mood) {
      smoothMood -= fadeSpeed;
      if (smoothMood < mood)
        smoothMood = mood;
    }

    showLEDState(smoothMood);
  }
}

void showLEDState(float state) {

  int red = 0;
  int green = 0;
  int blue = 0;

  if (state <= 5) {
    // BLUE -> CYAN
    red = 0;
    green = state * 51;
    blue = 255;
  }
  else if (state <= 10) {
    // CYAN -> GREEN
    red = 0;
    green = 255;
    blue = 255 - (state - 5) * 51;
  }
  else if (state <= 15) {
    // GREEN -> YELLOW
    red = (state - 10) * 51;
    green = 255;
    blue = 0;
  }
  else {
    // YELLOW -> RED
    red = 255;
    green = 255 - (state - 15) * 51;
    blue = 0;
  }

  analogWrite(RledPin, red);
  analogWrite(GledPin, green);
  analogWrite(BledPin, blue);
}
