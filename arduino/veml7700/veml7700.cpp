#include "Adafruit_VEML7700.h"

Adafruit_VEML7700 veml = Adafruit_VEML7700();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Adafruit_VEML7700 test");

  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");

  Serial.print("Gain: ");
  switch(veml.getGain()) {
    case VEML7700_GAIN_1:
      Serial.println("1");
      break;
    case VEML7700_GAIN_2:
      Serial.println("2");
      break;
    case VEML7700_GAIN_1_4:
      Serial.println("1/4");
      break;
    case VEML7700_GAIN_1_8:
      Serial.println("1/8");
      break;
  }

  Serial.print("Integration time (ms): ");
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS:
      Serial.println("25");
      break;
    case VEML7700_IT_50MS:
      Serial.println("50");
      break;
    case VEML7700_IT_100MS:
      Serial.println("100");
      break;
    case VEML7700_IT_200MS:
      Serial.println("200");
      break;
    case VEML7700_IT_400MS:
      Serial.println("400");
      break;
    case VEML7700_IT_800MS:
      Serial.println("800");
      break;
  }

  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);
}

void loop() {
  uint16_t irq;
  
  Serial.print("raw ALS: ");   Serial.println(veml.readALS());
  Serial.print("raw white: "); Serial.println(veml.readWhite());
  Serial.print("lux: ");       Serial.println(veml.readLux());

  irq = veml.interruptStatus();
  if (irq & VEML7700_INTERRUPT_LOW) {
    Serial.println("** Low threshold");
  }
  if (irq & VEML7700_INTERRUPT_HIGH) {
    Serial.println("** High threshold");
  }
  delay(500);
}
