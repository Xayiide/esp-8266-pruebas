# Arduino
En este directorio se encontrarán los ficheros usados para hacer las pruebas con Arduino. Para grabar los ejemplos basta abrirlos usando el IDE de Arduino y seleccionar la placa y el puerto adecuados. Salvo que se especifique otra cosa, las placas utilizadas son un Arduino UNO y un NodeMCU con un procesador ESP-8266 de Espressif.

# Prueba 0:
Primera prueba. Hace que los LEDs incluidos en la placa NodeMCU se enciendan y apaguen intermitentemente cada medio segundo. Un LED está mapeado al GPIO 2 y otro al GPIO 16.

# Prueba 1:
Se prueba la conexión WiFi del NodeMCU. Se consigue conectar a la red compartida por un smartphone.

# Prueba 2:
Esta prueba hace uso de un Arduino UNO para probar la conexión por UART. El puerto UART está mapeado con el USB al que se conecta el UNO para recibir corriente desde el PC. Desde el IDE de Arduino se puede abrir el puerto serie para enviar información por UART al Arduino UNO. Cuando le enviamos un '1', enciende el LED conectado al GPIO 13. Cuando le enviamos un '0', lo apaga.  
![](https://github.com/Xayiide/esp-8266-pruebas/blob/main/assets/UnoUART.png)

# Prueba 3:
Esta prueba hace uso tanto del NodeMCU como del Arduino UNO. El Arduino se conectará al PC por su puerto de USB (mapeado a UART). Por ahí será por donde recibirá los unos y ceros, como en la [prueba 2](#prueba-2). Pero ahora en lugar de activar directamente el LED, el Arduino enviará lo que ha recibido del usuario al NodeMCU, que será el que active el LED siguiendo la misma lógica ('1' para encenderlo, '0' para apagarlo).
![](https://github.com/Xayiide/esp-8266-pruebas/blob/main/assets/NodeUnoUART.png)


(Faltan las pruebas 4 y 5)

# Prueba 6:
Prueba rápida de una pantalla LCD 1602A.

