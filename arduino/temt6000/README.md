# Funcionamiento del sensor temt6000

Es un sensor de luz. La tensión de entrada puede ser de 3.3 a 5 voltios. Lo que se escoja será importante para escalar el valor leído.

El sensor consiste en un fototransistor que aumenta la intensidad de salida cuanta más luz haya. El valor máximo de lectura dependerá de la tensión de entrada. En este ejemplo se suministrarán 5 voltios, así que la lectura máxima será de 5 V cuando haya mucha luz. Como el sensor incorpora una resistencia de 10k ohmnios, la lectura se hará en voltaje.

![](https://github.com/Xayiide/esp-8266-pruebas/blob/main/assets/temt6000_schem.jpg)

Cuando se utiliza un voltaje de referencia de 5 V, las lecturas con un multímetro coinciden con las lecturas del ADC de Arduino. Sin embargo, cuando se utiliza 3.3 V empieza a haber una diferencia de entre ~20-30 mV en las lecturas.


# Recursos
[Esta](https://protosupplies.com/product/temt6000-ambient-light-sensor-module/) es una guía muy buena.
[Este es un vídeo muy bueno](https://www.youtube.com/watch?v=HwQFhM8IdeQ)
[Este es el manual del temt6000](https://www.vishay.com/docs/81579/temt6000.pdf)
