class LED {
private:
  int pin;

public:
  // constructor: initialize with pin number
  LED(int pinNumber) {
    pin = pinNumber;
    pinMode(pin, OUTPUT);
  }

  // turn the LED on
  void on() { digitalWrite(pin, HIGH); }

  // turn the LED off
  void off() { digitalWrite(pin, LOW); }

  // blink the LED with a given delay time
  void blink(int delayTime) {
    on();
    delay(delayTime);
    off();
    delay(delayTime);
  }
};

// create LED objects for pins 13, 12, 11, 10
LED leds[] = {LED(13), LED(12), LED(11), LED(10)};
const int numLeds = sizeof(leds) / sizeof(leds[0]);

void setup() {
  // all pins are already set as OUTPUT by the constructor
}

void loop() {
  // turn all LEDs on
  for (int i = 0; i < numLeds; i++) {
    leds[i].on();
  }
  delay(1000);

  // turn all LEDs off
  for (int i = 0; i < numLeds; i++) {
    leds[i].off();
  }
  delay(1000);
}
