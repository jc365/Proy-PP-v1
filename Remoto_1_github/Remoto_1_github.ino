/*----------------------------------------------------------------------------
  PROGRAMACION DEL REMOTO 1 (unido via I2C al principal)
  
  Sensores / actuadores conectados:
  - Sensor de temperatura DHT22 para registrar temp/hume interior
  - 2 reles 
  - Modulo de transmision: 
    a) pruebas, HC05 bluetooth, 
    b) productivo, GSM
-----------------------------------------------------------------------------*/ 
// Librerias utilizadas
#include <DHT.h>
#include <SoftwareSerial.h>
#include <Wire.h>
// Constantes 
#define VERSION "R1f5v1 -> "
#define DBGLVL  3
#define CHN_BT  
#undef  CHN_ZIG 
#define CHN_SRL 
#define CHN_I2C 
#define MI_ID   1       // Identificador del modulo (id=0 para MASTER; id=1 para REMOTO1; ...)
#define WIREDST 0 
//-
#define DhtGND  8       // DHT22 - pin GND 
#define DhtPIN  9       // DHT22 - pin de lectura
#define DhtVCC  10      // DHT22 - pin VCC 
#define DhtTYPE DHT22   // DHT22-  tipo de sensor de temperatura y humedad 
//-
#define BtTX    3       // HC05 - pin TX
#define BtRX    4       // HC05 - pin RX
//-
#define releENT 12      // Pin de control del Rele 1 (Entrada)
#define releCLF 13      // Pin de control del Rele 2 (Calefaccion)
// Variables Globales
SoftwareSerial Bt(BtTX, BtRX); 
DHT            Dht(DhtPIN, DhtTYPE);

void setup(){    // --- SETUP del Remoto1 ---
  // activar pines Reles a HIGH (apagados)
  pinMode(releENT, OUTPUT);  digitalWrite(releENT, HIGH);
  pinMode(releCLF, OUTPUT);  digitalWrite(releCLF, HIGH);
  // activar pines DHT22
  pinMode(DhtVCC, OUTPUT);   digitalWrite(DhtVCC,  HIGH);
  pinMode(DhtGND, OUTPUT);   digitalWrite(DhtGND,   LOW);
  // inicializa el DHT22, el Serial y el I2C
  Dht.begin();    
  Serial.begin(9600);
  Wire.begin(MI_ID);            
  Wire.onReceive(leer_I2C);     // Al recibir una petición, ejecuta rutina "leer_I2C"
  log_v2(1,"Inicializando Remoto 1",-99,-99,-99,-99); 
  delay(2000);
}


void loop() {    // --- LOOP del Remoto1 ---
  if (Serial.available()) leer_Serial();  // Permite el envio de MSG-prueba por consola 
  if (Bt.available())     leer_Bt();      // Entrada inalambrica de MSGs
}


