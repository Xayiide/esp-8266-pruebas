#include <SoftwareSerial.h>

SoftwareSerial espUart(2, 3); /* Rx: 2, Tx: 3 */

void setup() {
  Serial.begin(9600);
  espUart.begin(9600);
}

void loop() {
  char data;
  // Leer del serial-usb conectado al PC
  if (Serial.available() > 0) {
    Serial.println();
    Serial.print("[+] Recibido del PC: ");
    while (Serial.available() > 0) {
      data = Serial.read();
      Serial.print(data);
      espUart.print(data); // Enviar lo leído al ESP8266
      delay(5);
    }
  }
  delay(100);
}
