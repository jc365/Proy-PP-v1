/*
 * Blink
 * Turns on an LED on for one second,\
 * then off for one second, repeatedly.
 */

#define DBGLVL  1

#include <Arduino.h>
#include <DBGmacros.h>
#include <Prototipado.h>


void setup() {
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop(/* arguments */) {
  /* code */
  for (size_t x = 1; x < 10; x++) {
    parpadear(x);
    delay(x*300);
  }
  Serial.print(FECHA_HORA_Y4);
}


void parpadear(int segundos)
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(multiplicar(segundos,100));
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
   // wait for a second
  delay(multiplicar(segundos, 50));
  DBGLOG_F2("Parpadeando. Segundos: ",segundos);
}

int multiplicar(int n, int factor) {
  return n*factor;
}
