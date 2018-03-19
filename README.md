# Proy-PP-v1
Programación para domotica 


describir los sensores y actuadores conectados






Detalles y restricciones de libreria:
// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Uno        9         8         10
//
// El HC-05 (bluetooth) está conectado a los pines: 8 (TX) y 9 (RX)



-----------------------------------------------------------------------------
Electrocooking nov 2016 - Luchyam
Sensor de temperatura y humedad DHT22 con un display LCD 16 x 2 - v1.0
----------------------------------------------------------------------------- 
-- DISPLAY 16x2 ---
Conexiones :
* LCD RS pin to digital pin 12
* LCD Enable pin to digital pin 11
* LCD D4 pin to digital pin 5
* LCD D5 pin to digital pin 4
* LCD D6 pin to digital pin 3
* LCD D7 pin to digital pin 2
* LCD R/W pin to ground
* LCD VSS pin to ground
* LCD VCC pin to 5V
* 10K resistor:
* ends to +5V and ground
* wiper to LCD VO pin (pin 3)
Para más detalles de la librería del display y de su uso:
   http://www.arduino.cc/en/Tutorial/LiquidCrystal
 
