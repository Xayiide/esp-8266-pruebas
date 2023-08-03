#define LED_OUT 13

static int value = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_OUT, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    value = Serial.read();
    delay(5);
  }

  if (value == '1') {
    digitalWrite(LED_OUT, HIGH);
  }
  else if (value == '0') {
    digitalWrite(LED_OUT, LOW);
  }
}
