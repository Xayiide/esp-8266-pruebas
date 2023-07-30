# Instrucciones

## Añadir esp8266 a Arduino
https://github.com/esp8266/arduino#installing-with-boards-manager

## Para instalar el entorno de programación SDK de espressif para el 8266
https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html#introduction

y particularmente:

https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html

### python-serial
No encuentra el paquete. Puede que haya que instalarlo después. Mientras tanto instalo python3-serial

### Problemas con pip
Para instalar pip en python2.7:

https://linuxhint.com/install-pip3-and-pip2-debian/

### make menuconfig
hay que instalar `gperf`.

### Final
Siguiendo las instrucciones podemos terminar con el make flash y el make monitor

Se sale pulsando <kbd>Ctrl</kbd> + <kbd>]</kbd>, que en teclado español es <kbd>Ctrl</kbd> + <kbd>Alt</kbd> + <kbd>]</kbd> (la tecla dos a la derecha de la *p*, que tiene _* + ]_)

Para poder compilar el proyecto hay que exportar al PATH la ruta donde está `esp/xtensa-lx106-elf/bin` de acuerdo a [esto](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html#toolchain-setup)
