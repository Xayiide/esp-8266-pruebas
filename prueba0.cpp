/* Esto está hecho y compilado con el IDE de Arduino para la placa NodeMCU,
 * con micro esp8266 de Espressif. Enciende las dos luces que tiene la placa,
 * el LED 1 y el LED 2.
 */

#define LED1 D0
#define LED2 D4


void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(500);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  delay(500);
}

