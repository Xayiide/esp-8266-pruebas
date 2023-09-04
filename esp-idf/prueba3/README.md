# Prueba 3:
En esta prueba se programará un NodeMCU con el SDK de Espressif y un Arduino UNO con el IDE de Arduino. El propósito será que el Arduino esté conectado al ordenador por el puerto USB, a su vez mapeado a la UART. Desde el IDE de Arduino abriremos el puerto serial y enviaremos mensajes al UNO que serán reenviados al NodeMCU. Si el mensaje es un '1', encenderá un LED colocado en el GPIO 13. Si es un '0', lo apagará.

## Diagrama:
![](https://github.com/Xayiide/esp-8266-pruebas/blob/main/assets/NodeUnoUART)

