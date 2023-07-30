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
Este ejemplo es [este](https://github.com/espressif/ESP8266_RTOS_SDK/tree/af0cdc36fa2600033d0a09301c754008cf1503c1/examples/get-started/hello_world).
