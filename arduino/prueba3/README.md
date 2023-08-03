# NodeUART
Su papel es leer lo que le manda la placa Uno. Si lee un '1', enciende el LED conectado al GPIO 13. Si lee un '0', lo apaga.

# UnoUART
Su papel es leer lo que se le manda desde el PC, conectado por el puerto USB de carga y usado a modo de UART. Le reenvía a la placa Node lo que nosotros le enviamos.

# Diagrama
![](https://github.com/Xayiide/esp-8266-pruebas/blob/main/assets/NodeUnoUART.png)
