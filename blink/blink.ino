class LED {
private:
  int pin;

public:
  LED(int pinNumber) {
    pin = pinNumber;
    pinMode(pin, OUTPUT);
  }

  // LEDs D1 to D4 are L-active
  void on() { digitalWrite(pin, LOW); }
  void off() { digitalWrite(pin, HIGH); }
};

class Button {
private:
  int pin;

public:
  Button(int pinNumber) {
    pin = pinNumber;
    pinMode(pin, INPUT_PULLUP);
  }

  bool isPressed() { return digitalRead(pin) == LOW; }
};

// LEDs on pins 13, 12, 11, 10
LED leds[] = {LED(13), LED(12), LED(11), LED(10)};
const int numLeds = sizeof(leds) / sizeof(leds[0]);

// Buttons on A1 (LSB), A2, A3 (MSB)
Button button1(A1);
Button button2(A2);
Button button3(A3);

// Turn off all LEDs
void turnAllOff() {
  for (int i = 0; i < numLeds; i++) {
    leds[i].off();
  }
}

// === LED Patterns ===

void allOff() {
  // 000: All off
  turnAllOff();
}

void chaseForward() {
  // 001: Chase forward
  for (int i = 0; i < numLeds; i++) {
    leds[i].on();
    delay(100);
    leds[i].off();
  }
}

void chaseBackward() {
  // 010: Chase backward
  for (int i = numLeds - 1; i >= 0; i--) {
    leds[i].on();
    delay(100);
    leds[i].off();
  }
}

void outsideIn() {
  // 011: Outside-in blink
  leds[0].on();
  leds[3].on();
  delay(200);
  leds[0].off();
  leds[3].off();
  leds[1].on();
  leds[2].on();
  delay(200);
  leds[1].off();
  leds[2].off();
}

void pingPong() {
  // 100: Ping-pong blink (ends where it starts)
  for (int i = 0; i < numLeds; i++) {
    leds[i].on();
    delay(100);
    leds[i].off();
  }
  for (int i = numLeds - 2; i > 0; i--) {
    leds[i].on();
    delay(100);
    leds[i].off();
  }
}

void snake() {
  // 101: Snake left to right (cumulative)
  for (int i = 0; i < numLeds; i++) {
    leds[i].on();
    delay(150);
  }
  for (int i = 0; i < numLeds; i++) {
    leds[i].off();
    delay(150);
  }
}

void all() {
  // 110: All blink together
  for (int i = 0; i < numLeds; i++)
    leds[i].on();
  delay(300);
  for (int i = 0; i < numLeds; i++)
    leds[i].off();
  delay(300);
}

void randomFlicker() {
  // 111: Random LED flicker
  int r = random(numLeds);
  leds[r].on();
  delay(100);
  leds[r].off();
  delay(100);
}

void setup() {
  Serial.begin(9600); // Beginn serial
  delay(500); // Time for Serial to connect
  Serial.println(F("Onboard LED Blinking"));
  Serial.end(); // End serial

  randomSeed(analogRead(0)); // Seed for random pattern
}

void loop() {
  bool b1 = button1.isPressed(); // A1 = LSB
  bool b2 = button2.isPressed();
  bool b3 = button3.isPressed(); // A3 = MSB

  byte pattern = (b3 << 2) | (b2 << 1) | (b1 << 0);

  switch (pattern) {
    case 0b000: allOff(); break;
    case 0b001: chaseForward(); break;
    case 0b010: chaseBackward(); break;
    case 0b011: outsideIn(); break;
    case 0b100: pingPong(); break;
    case 0b101: snake(); break;
    case 0b110: all(); break;
    case 0b111: randomFlicker(); break;
  }
}
