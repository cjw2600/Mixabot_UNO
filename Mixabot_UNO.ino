#include <SoftwareSerial.h>

SoftwareSerial espSerial(10, 11); //RX, TX

const char* ssid = "BG_Mixed";
const char* password = "uptotheelbow";

char ip_address[16];


char serial_input[64];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74880);
  while (!Serial) {
    ;// Wait for serial port to connect. Needed for native USB port only.
  }
  Serial.println("UNO, reporting for duty.");

  espSerial.begin(9600);
  delay(10);

  Serial.println("waiting for the ESP to start up...");

  for (;;) {
    memset(serial_input, 0, sizeof(serial_input));
    int characters_received = espSerial.readBytesUntil('\n', serial_input, sizeof(serial_input));
    if (characters_received > 0) {
      Serial.print("Received ");
      Serial.print(characters_received);
      Serial.print(" characters from the ESP. Reprinting: ");
      Serial.println(serial_input);
      if (0 == strncmp(serial_input, "startup", sizeof("startup") - 1)) {
        break;
      }
    }
  }
  for (;;) {
    memset(serial_input, 0, sizeof(serial_input));
    int characters_received = espSerial.readBytesUntil('\n', serial_input, sizeof(serial_input));
    if (characters_received > 0) {
      int compare = strncmp(serial_input, "startup", sizeof("startup") - 1);
      if (0 == compare) {
        Serial.println("Sending credentials...");
        delay(100);
        espSerial.print("startup\n");
        delay(100);
        espSerial.print(ssid);
        espSerial.print("\n");
        delay(100);
        espSerial.print(password);
        espSerial.print("\n");
        break;
      } else {
        Serial.print("Got unknown startup message: ");
        Serial.println(serial_input);
      }
    }
  }
  
  bool server_started = false;
  Serial.println("Credentials sent. Waiting for ESP server to start...");
  
  for(;;) {
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
        memcpy(ip_address, serial_input, sizeof(serial_input));
        ip_address[sizeof(ip_address) - 1] = '\0';
        break;
      } else {
        Serial.print("received ");
        Serial.print(characters_received);
        Serial.print(" characters. strncmp reports ");
        Serial.print(compare);
        Serial.print(". String: ");
        Serial.println(serial_input);
      }
    } else {
      Serial.println("Timed out waiting to receive anything from the ESP.");
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
