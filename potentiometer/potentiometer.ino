#define Pot1 0

void setup() {
  Serial.begin(9600); Serial.println("--- Multi-Function-Shield Potentiometer ---");
}


void loop() {
  Serial.print("Potentiometer reading: ");
  Serial.println(analogRead(Pot1));

  delay(500);
}
