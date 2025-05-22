#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

const byte SEGMENT_SELECT[4] = {0xF1, 0xF2, 0xF4, 0xF8}; // Active-low: 11110001 etc.

byte segmentStates[4] = {0xFF, 0xFF, 0xFF, 0xFF}; // All OFF (1 = off, 0 = on)

String inputBuffer = "";

void setup() {
  pinMode(LATCH_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);
  Serial.begin(9600);

  delay(500); // Give time for Serial to connect
  Serial.println(F("7-Segment Display Controller"));
  Serial.println(F("--------------------------------"));
  Serial.println(F("Control individual segments of 4 digits."));
  Serial.println(F("Format: [digit] [segment] [1=on/0=off]"));
  Serial.println(F("Example: 2 3 1  → Turn ON segment 3 of digit 2"));
  Serial.println(F("Segments:"));
  Serial.println(F(" 0 = A"));
  Serial.println(F(" 1 = B"));
  Serial.println(F(" 2 = C"));
  Serial.println(F(" 3 = D"));
  Serial.println(F(" 4 = E"));
  Serial.println(F(" 5 = F"));
  Serial.println(F(" 6 = G"));
  Serial.println(F(" 7 = Dot (DP)"));
  Serial.println(F("--------------------------------\n"));
}

void loop() {
  static unsigned long lastUpdate = 0;
  static byte currentDigit = 0;

  // Refresh 
  if (millis() - lastUpdate >= 1) {
    updateDigit(currentDigit);
    currentDigit = (currentDigit + 1) % 4;
    lastUpdate = millis();
  }

  // Read serial input 
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      processInput(inputBuffer);
      inputBuffer = "";
    } else {
      inputBuffer += c;
    }
  }
}

void updateDigit(byte digit) {
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, segmentStates[digit]);     // Segment data
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[digit]);    // Digit selector
  digitalWrite(LATCH_DIO, HIGH);
}

void processInput(String input) {
  input.trim(); // remove \r, \n, and spaces
  if (input.length() == 0) return; // ignore empty input

  int space1 = input.indexOf(' ');
  int space2 = input.indexOf(' ', space1 + 1);
  if (space1 == -1 || space2 == -1) {
    Serial.println(F("Error: Format must be '[digit] [segment] [1/0]'"));
    return;
  }

  int digit = input.substring(0, space1).toInt();
  int segment = input.substring(space1 + 1, space2).toInt();
  int state = input.substring(space2 + 1).toInt();

  if (digit < 0 || digit > 3) {
    Serial.println(F("Error: Digit must be between 0 and 3"));
    return;
  }
  if (segment < 0 || segment > 7) {
    Serial.println(F("Error: Segment must be between 0 and 7"));
    return;
  }
  if (state != 0 && state != 1) {
    Serial.println(F("Error: State must be 1 (on) or 0 (off)"));
    return;
  }

  if (state == 1) {
    segmentStates[digit] &= ~(1 << segment); // ON
    Serial.print(F("Turned ON segment "));
  } else {
    segmentStates[digit] |= (1 << segment);  // OFF
    Serial.print(F("Turned OFF segment "));
  }
  Serial.print(segment);
  Serial.print(F(" on digit "));
  Serial.println(digit);
}
