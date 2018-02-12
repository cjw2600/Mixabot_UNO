#include <SoftwareSerial.h>

SoftwareSerial espSerial(10, 11); //RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74880);
  while (!Serial) {
    ;// Wait for serial port to connect. Needed for native USB port only.
  }
  Serial.println("UNO, reporting for duty.");

  espSerial.begin(9600);
}

void loop() {
  // Ping the ESP to see if there are any orders
  espSerial.println("u");
  delay(2000);
  char serialInput[64];
  int characters_received = espSerial.readBytesUntil('\n', serialInput, sizeof(serialInput));
  if (characters_received > 0) {
    Serial.print(serialInput);
  } else {
    Serial.println("Got no response from the ESP");
  }
}
