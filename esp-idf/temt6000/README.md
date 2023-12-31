# Funcionamiento del sensor temt6000

Es un sensor de luz. La tensión de entrada puede ser de 3.3 a 5 voltios. Lo que se escoja será importante para escalar el valor leído.

El sensor consiste en un fototransistor que aumenta la intensidad de salida cuanta más luz haya. El valor máximo de lectura dependerá de la tensión de entrada. En este ejemplo se suministrarán 5 voltios, así que la lectura máxima será de 5 V cuando haya mucha luz. Como el sensor incorpora una resistencia de 10k ohmnios, la lectura se hará en voltaje.

![](https://github.com/Xayiide/esp-8266-pruebas/blob/main/assets/temt6000_schem.jpg)

El esp8266 sólo puede alimentar el temt6000 con 3.3 V. Como pasa en el caso de Arduino, la lectura con 3.3 V no es demasiado precisa, no sé bien por qué. Ahora mismo devuelve el voltaje con una precisión de unos +-10 mV.


# Recursos
[Esta](https://protosupplies.com/product/temt6000-ambient-light-sensor-module/) es una guía muy buena.
[Este es un vídeo muy bueno](https://www.youtube.com/watch?v=HwQFhM8IdeQ)
[Este es el manual del temt6000](https://www.vishay.com/docs/81579/temt6000.pdf)
