#include <SoftwareSerial.h>
#include <AltSoftSerial.h>

// INI - definiciones LCD
#include <DHT.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>

//-INI cabecera del programa unitario de lectura y logica de luz
#define DIA      0                           // Estos valores DIA/NOCHE estan ajustados para que
#define NOCHE -100                           // sumado al umbral fije un margen de tolerancia para evitar encender/apagar por pequeños cambios
// variables del modulo LDR (fotoresistencia)
int LDR_umbral_luz = 900 ;                   // No lo pongo en #define por si hago un servicio para tarar el valor
int LDR_lectura = 0 ;
int LDR_estado = NOCHE ;
int LDR_corte_operativo = LDR_umbral_luz ;
int LDR_sw_ent = "C" ;                       // S=encendido / N=apagado / C=crepuscular
int LDR_sw_frl = "S" ;                       // S=encendido / N=apagado / C=crepuscular
int LDR_ult_tto = DIA ;
// variables para simular entrada y trazar logica
int test[] = {901, 890, 790, 810, 901} ;
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
//-FIN cabecera del programa unitario de lectura y logica de luz


/*
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
 
-- DHT22 ---
Conexiones :
* DHT22 data pin to digital pin 7
----------------------------------------------------------------------------- 
*/
 
#define DHTPIN 7        // Digital pin al que está conectado el DHT22
#define DHTTYPE DHT22   // Tipo de sensor DHT22  (AM2302), AM2321
 
// inicializa la libreria con el numero de los pins de la interface
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);
int dlt_lectura = 2000 ;
int cont_registro = 0 ;
int temp_int, temp_ext, hume_int, hume_ext ;
int tMax = -90;   // Variables para registrar valores minimo y Maximo
int tMin = 200; 
String fecha, hora, msg2;
byte grado[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
// FIN - definiciones LCD

#define VERSION "Mf4 -> "
#define DBGLVL 3

// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Uno        9         8         10
//
// El HC-05 (bluetooth) está conectado a los pines: 8 (TX) y 9 (RX)

// Constantes 

#define DhtGND  8       // Pin GND del DHT22
#define DhtPIN  9       // Pin de lectura del DHT22
#define DhtVCC  10      // Pin VCC del DHT22
#define DhtTYPE DHT22   // Tipo de sensor de temperatura y humedad (DHT22)  
//#define ZigVCC  2       // Detalle de los pines ...
//#define ZigTX   3       // a los que conectado el ...
//#define ZigRX   4       // Remote ZigBee CC2530 ...   
//#define ZigGND  5       // 2.4G Serial 
#define ZigVCC  15       // Detalle de los pines ...
#define ZigTX   16       // a los que conectado el ...
#define ZigRX   17       // Remote ZigBee CC2530 ...   
#define ZigGND  18  
#define releFAR 12      // Pin de control del Rele 1 (Farolas)
#define releCST 13      // Pin de control del Rele 2 (Caseta)
#define releCHM 6       // Pin de control del Rele 3 (Chamizo)
#define releCAR 7       // Pin de control del Rele 4 (Caravana)
#define MI_ID   0       // Identificador del modulo ZigBee (id=0 para MASTER; id=1 para REMOTO1)

SoftwareSerial Zig(ZigTX, ZigRX);
AltSoftSerial  Bt;

String str, srv, temp, hume;
int a = 0;

void setup() {
  // activar pines de alimentacion
  pinMode(ZigVCC, OUTPUT);  digitalWrite(ZigVCC, HIGH);
  pinMode(ZigGND, OUTPUT);  digitalWrite(ZigGND, LOW);
  Zig.begin(57600);
  Serial.begin(9600);
  Bt.begin(38400);  
  //delay(2000); 
  log_v2(1,"Arranque de la placa CENTRAL...",-99,-99,-99,-99);  
}  

void loop() {
//  Medir_Sondas_Locales(dlt_lectura);
//  leer_luz();
  delay(1600);                            // Calculado por tanteo
  if (Serial.available()) leer_Serial();  // Permite el envio de MSG-prueba por consola 
  if (Zig.available())    leer_Zig();     // Salida/Contestacion de MSGs con remotos
  if (Bt.available())     leer_Bt();      // Entrada productiva de MSGs
}


void leer_Serial() {
  log_v2(2,"Entrando en leerSerial()",-99,-99,-99,-99);
  String msg = "";
  char inChar;
  // posicionamiento al principio del mensaje
  while (Serial.available() && inChar != '#' ) {
    inChar = Serial.read();
    log_v2(3,"Serial- Leido y desechado: "+inChar,-99,-99,-99,-99);
  } 
  // carga msg hasta final del mensaje
  while (Serial.available() && inChar != '/' ) {
    inChar = Serial.read();
    msg += inChar;
  }  
  log_v2(1,"MSG leido: #"+msg,-99,-99,-99,-99);
  formatear_msg(msg);
}

void leer_Zig() {
  log_v2(2,"Entrando en leerZig()",-99,-99,-99,-99);
  String msg = "";
  char inChar;
  // posicionamiento al principio del mensaje
  while (Zig.available() && inChar != '#' ) {
    inChar = Zig.read();
    log_v2(3,"Zig- Leido y desechado: "+inChar,-99,-99,-99,-99);
  }  
  // carga msg hasta el final del mensaje
  while (Zig.available() && inChar != '/' ) {
    inChar = Zig.read();
    msg += inChar;
  }  
  log_v2(1,"MSG leido: #"+msg,-99,-99,-99,-99);
  formatear_msg(msg);
}

void leer_Bt() {
  log_v2(2,"Entrando en leerBt()",-99,-99,-99,-99);
  String msg = "";
  char inChar;
  // posicionamiento al principio del mensaje
  while (Bt.available() && inChar != '#' ) {
    inChar = Bt.read();
    log_v2(3,"Bt- Leido y desechado: "+inChar,-99,-99,-99,-99);
  }  
  // carga msg hasta el final del mensaje
  while (Bt.available() && inChar != '/' ) {
    inChar = Bt.read();
    msg += inChar;
  }  
  log_v2(1,"MSG leido: #"+msg,-99,-99,-99,-99);
  formatear_msg(msg);
  Medir_Sondas_Locales(dlt_lectura);
  leer_luz();
}
