//
//    FILE: AM2315C.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: Demo for AM2315C I2C humidity & temperature sensor
//

//  Always check datasheet
//
//                    +-----------------+
//    RED    -------- | VDD             |
//    YELLOW -------- | SDA    AM2315C  |
//    BLACK  -------- | GND             |
//    WHITE  -------- | SCL             |
//                    +-----------------+


#include "AM2315C.h"

AM2315C DHT;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("AM2315C LIBRARY VERSION: ");
  Serial.println(AM2315C_LIB_VERSION);
  Serial.println();

  Wire.begin();
  Wire.setClock(400000);
  DHT.begin();

  delay(1000);
}


void loop()
{
  if (millis() - DHT.lastRead() >= 1000)
  {
    //  READ DATA
    int      ret    = DHT.read();
    int      status = DHT.readStatus();


    //  DISPLAY DATA, sensor has only one decimal.
    Serial.print("Humedad: ");
    Serial.println(DHT.getHumidity(), 1);
    Serial.print("Temperatura: ");
    Serial.println(DHT.getTemperature(), 1);
    Serial.print("Status: ");
    Serial.println(status);
  }
}


//  -- END OF FILE --
