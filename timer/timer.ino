#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
const byte SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

unsigned int counter = 0;
unsigned long lastUpdate = 0;

void setup()
{
  pinMode(LATCH_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);
}

void loop()
{
  // Update display
  for (int i = 0; i < 50; i++) {
    WriteNumberToSegment(0, (counter / 1000) % 10);
    WriteNumberToSegment(1, (counter / 100) % 10);
    WriteNumberToSegment(2, (counter / 10) % 10);
    WriteNumberToSegment(3, counter % 10);
  }

  // Wait for 1 second to increment counter
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    counter++;
    if (counter > 9999) counter = 0;  // Reset after 9999
  }
}

void WriteNumberToSegment(byte Segment, byte Value)
{
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment]);
  digitalWrite(LATCH_DIO, HIGH);
}
