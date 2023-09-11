# ESP-IDF
En este directorio se encontrará todo lo realizado con el entorno de 
ESP-IDF. Las instrucciones para montar el entorno se encuentran en estas
dos páginas:
- [ESP8266-RTOS-SDK](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html)
  - [ESP8266-RTOS-SDK para Linux](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html)
  - [Ejemplos de uso](https://github.com/espressif/ESP8266_RTOS_SDK/tree/af0cdc36fa2600033d0a09301c754008cf1503c1/examples)

# Usar el entorno
Para usar el entorno primero hay que ejecutar el comando `get_lx106`, que será un alias en nuestro *.bashrc* a `export PATH="$PATH:$HOME/esp/xtensa-lx106-elf/bin`.

Luego podremos ejecutar los `make` adecuados, como `make flash` o `make monitor`.

## Nota
Para salir del `make monitor` hay que pulsar <kbd>Ctrl</kbd> + <kbd>Alt Gr</kbd> + <kbd>]</kbd>



# hello\_world
Este ejemplo sale de [aquí](https://github.com/espressif/ESP8266_RTOS_SDK/tree/af0cdc36fa2600033d0a09301c754008cf1503c1/examples/get-started/hello_world). Es un ejemplo básico que sólo imprime un poco de información por la UART.

# Prueba 0:
Este ejemplo es el equivalente usando el entorno de ESP-IDF a [arduino/prueba0](../arduino/prueba0/prueba0.cpp). Hace que los dos LEDs del NodeMCU se enciendan intermitentemente cada medio segundo. Cuando uno se enciende, el otro se apaga.

# Prueba 1:
Este ejemplo es el equivalente usando el entorno de ESP-IDF a [arduino/prueba1](../arduino/prueba1/prueba1.cpp). El NodeMCU se conecta a la WiFi descrita y enciende las luces como en la prueba0.

# Prueba 2:
Este ejemplo es el equivalente usando el entorno de ESP-IDF a [arduino/prueba2](../arduino/prueba2/prueba2.cpp). Usando un Arduino UNO, si recibe un '1' por la UART enciende el LED conectado al GPIO 13, y lo apaga si recibe un '0'.
![](https://github.com/Xayiide/esp-8266-pruebas/blob/main/assets/NodemcuUART.png)

> En lugar de ser un AdaFruit es un NodeMCU de Spressif

En este ejemplo es importante aclarar dos cosas, que servirán también para la prueba 3:
1- No se puede grabar la placa si están los pines de Rx (GPIO3) y Tx (GPIO1) conectados.
2- No se pueden enviar datos desde el ordenador si están los pines Rx y Tx usados por otro lado.

# Prueba 3:
Este ejemplo es el equivalente usando el entorno de ESP-IDF a [arduino/prueba3](../arduino/prueba3/). Hay un README dentro de la carpeta que lo explica mejor.

# Prueba 4:
Prueba no terminada. No tiene correspondiente con Arduino. Es una prueba de servidor y cliente HTTP.

# Prueba 5:
Por ahora no tiene correspondencia en Arduino. Es una prueba de fotorresistencias y sensores lumínicos.
