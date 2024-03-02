//serial uno receive data
int LED = 7;
void setup() {
  pinMode(LED, OUTPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  if (Serial.available()) {
    char receivedChar = Serial.read(); // Read the incoming byte
    Serial.write(receivedChar); // Echo the received byte back

    if (receivedChar == 'b0') {
      digitalWrite(LED, HIGH);
      Serial.println("\nweb mode"); // Print "web mode" if 'a' is received
    }else if (receivedChar == 'b1'){
      Serial.println("\nself control");
      digitalWrite(LED, LOW);
    }
  }
}
