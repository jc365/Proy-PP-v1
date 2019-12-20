/*----------------------------------------------------------------------------
  PROGRAMACION DEL REMOTO 2 (instalado en caseta)
  
  Sensores / actuadores conectados:
  - Sensor de temperatura DHT22 para registrar temp/hume exterior
  - 4 reles 
  - Sensor de luz, en puente con resistencia de xx ohmios
  - Modulo de transmision Remote ZigBee CC2530 2.4G Serial 
    configurado en modo Broadcast y velocidad 57600
-----------------------------------------------------------------------------*/ 
// Librerias utilizadas
#include <DHT.h>
#include <SoftwareSerial.h>
// Constantes 
#define VERSION "R2f5v1 -> "
#define DBGLVL  3
#define MI_ID   2       // Identificador del modulo (id=0 para MASTER; id=1 para REMOTO1; ...)
#define CHN_BT  false
#define CHN_ZIG true
#define CHN_SRL true
#define CHN_I2C false
#include "C:\Users\Juan\Documents\Arduino\DBGmacros\DBGmacros.ino"
//-
#define DhtGND  8       // DHT22 - pin GND 
#define DhtPIN  9       // DHT22 - pin de lectura
#define DhtVCC  10      // DHT22 - pin VCC 
#define DhtTYPE DHT22   // DHT22-  tipo de sensor de temperatura y humedad 
//-
#define ZigVCC  2       // ZigBee CC2530 - pin VCC
#define ZigTX   3       // ZigBee CC2530 - pin TX
#define ZigRX   4       // ZigBee CC2530 - pin RX
#define ZigGND  5       // ZigBee CC2530 - pin GND
//-
#define releFAR 12      // Pin de control del Rele 1 (Farolas)
#define releCST 13      // Pin de control del Rele 2 (Caseta)
#define releCHM 6       // Pin de control del Rele 3 (Chamizo)
#define releCAR 7       // Pin de control del Rele 4 (Caravana)
//-
#define DIA              0      // Estos valores DIA/NOCHE estan ajustados para que
#define NOCHE            -100   // sumado al umbral fije un margen de tolerancia para evitar encender/apagar por pequeños cambios
//- (GND)...LDR...(A0)...RESISTENCIA...(A5-D18)configurado a HIGH
#define LDRPIN           A0     // 
// Variables globales
int     LDR_umbral_luz = 900   ;             
int     LDR_lectura    = 0     ;
int     LDR_estado     = NOCHE ;
int     LDR_corte_op   = LDR_umbral_luz ;
int     LDR_ult_tto    = DIA   ;
int     temp           = 0     ;
int     hume           = 0     ;
String  msg                    ;
//-
SoftwareSerial Zig(ZigTX, ZigRX); 
DHT            Dht(DhtPIN, DhtTYPE);


void setup(){     // --- SETUP del Remoto2 ---
  // activar pines Reles a HIGH (apagados)
  pinMode(releFAR, OUTPUT);  digitalWrite(releFAR, HIGH);
  pinMode(releCST, OUTPUT);  digitalWrite(releCST, HIGH);
  pinMode(releCHM, OUTPUT);  digitalWrite(releCHM, HIGH);
  pinMode(releCAR, OUTPUT);  digitalWrite(releCAR, HIGH);
  // activar pines ZigBee
  pinMode(ZigVCC, OUTPUT);   digitalWrite(ZigVCC,  HIGH);  
  pinMode(ZigGND, OUTPUT);   digitalWrite(ZigGND,   LOW);
  // activar pines DHT22
  pinMode(DhtVCC, OUTPUT);   digitalWrite(DhtVCC,  HIGH);
  pinMode(DhtGND, OUTPUT);   digitalWrite(DhtGND,   LOW);
  // activar pin positivo del puente resistencia-LDR
  pinMode(18, OUTPUT);   digitalWrite(18,  HIGH);
    // Inicializa el DHT22, el ZigBee y el Serial
  Dht.begin();  
  Zig.begin(57600);  
  Serial.begin(9600);
  DBGLOG_F5("Inicializando sensor y Zigee, espera por favor. Valores int de HIGH/LOW: ",HIGH,LOW,-99,-99); 
  delay(2000);
}


void loop() {    // --- LOOP del Remoto2 ---
  if (Serial.available()) leer_Serial();  // Permite el envio de MSG-prueba por consola 
  if (Zig.available())    leer_Zig();     // Entrada productiva de MSGs
  if (millis() % 6500 == 0) 
    ejecutar_accion(1,0,0,0);
}


