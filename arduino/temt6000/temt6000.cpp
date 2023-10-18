#define TEMPTIN A0
#define AREF    5.0     /* Valor de voltaje de entrada */
#define INTRES  10000.0 /* Valor resistencia interna   */

void setup() {
  pinMode(TEMPTIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  float tempt_read, volts, amps, microamps, lux;
  float p;
  int   mV;
  
  tempt_read = analogRead(TEMPTIN);
  volts      = tempt_read * AREF / 1024.0;
  p          = map(tempt_read, 0, 1024, 0, AREF*1000);
  amps       = volts / INTRES;
  microamps  = amps * 1000000.0;
  lux        = microamps * 2.0; /* habría que sumar algo. Mirar datasheet */

  Serial.print("Lectura cruda: ");
  Serial.print(p);
  Serial.print(" -- ");
  Serial.println(tempt_read);
  Serial.print(volts);
  Serial.print(" V -- ");
  Serial.print(lux);
  Serial.println(" lx");
  

  delay(1000);
}
