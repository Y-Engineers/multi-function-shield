#define Pot1 0 // Potentiometer

// 7-segment Display Pins
#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

const byte SEGMENT_SELECT[4] = {0xF1, 0xF2, 0xF4, 0xF8}; // Active-low
byte segmentStates[4] = {0xFF, 0xFF, 0xFF, 0xFF};        // All segments off

// 7-segment encoding for digits 0–9 (common cathode)
const byte DIGIT_SEGMENTS[10] = {
  0b11000000, // 0
  0b11111001, // 1
  0b10100100, // 2
  0b10110000, // 3
  0b10011001, // 4
  0b10010010, // 5
  0b10000010, // 6
  0b11111000, // 7
  0b10000000, // 8
  0b10010000  // 9
};

void setup() {
  Serial.begin(9600);
  Serial.println(F("--- Potentiometer to 7-Segment Display ---"));

  pinMode(LATCH_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);
}

void loop() {
  static unsigned long lastDisplayUpdate = 0;
  static byte currentDigit = 0;
  static unsigned long lastRead = 0;

  // Read and clamp potentiometer value every 100ms
  if (millis() - lastRead > 100) {
    int potValue = analogRead(Pot1);
    int displayValue = potValue;
    if (displayValue > 9999) displayValue = 9999;
    if (displayValue < 0) displayValue = 0;

    Serial.print(F("Potentiometer: "));
    Serial.println(displayValue);

    setDisplayValue(displayValue);
    lastRead = millis();
  }

  // Refresh display via multiplexing
  if (millis() - lastDisplayUpdate >= 2) {
    updateDigit(currentDigit);
    currentDigit = (currentDigit + 1) % 4;
    lastDisplayUpdate = millis();
  }
}

void setDisplayValue(int value) {
  for (int i = 3; i >= 0; --i) {
    int digit = value % 10;
    segmentStates[i] = DIGIT_SEGMENTS[digit];
    value /= 10;
  }
}

void updateDigit(byte digit) {
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, segmentStates[digit]);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[digit]);
  digitalWrite(LATCH_DIO, HIGH);
}
