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

  Serial.println("waiting for the ESP to start up...");

  
  bool server_started = false;
  
  for(;;) {
    char serial_input[64];
    memset(serial_input, 0, sizeof(serial_input));
    int characters_received = espSerial.readBytesUntil('\n', serial_input, sizeof(serial_input));
    if (characters_received > 0) {
      serial_input[sizeof(serial_input)-1] = '\0';
      int compare = strncmp(serial_input, "Server started", sizeof("Server started") - 1);
      if (0 == compare) {
        Serial.println("ESP server started!");
        server_started = true;
        continue;
      } else if (server_started) {
        Serial.print("IP address: ");
        Serial.println(serial_input);
        break;
      } else {
        Serial.print("received ");
        Serial.print(characters_received);
        Serial.print(" characters. strncmp reports ");
        Serial.print(compare);
        Serial.print(". String: ");
        Serial.println(serial_input);
      }
    }
  }
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
