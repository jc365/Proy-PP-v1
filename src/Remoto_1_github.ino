/*----------------------------------------------------------------------------
  PROGRAMACION DEL REMOTO 1 (unido via I2C al principal)
  
  Sensores / actuadores conectados:
  - Sensor de temperatura DHT22 para registrar temp/hume interior
  - 2 reles 
  - Modulo de recepcion de instrucciones maestras: 
    a) pruebas, HC05 bluetooth, 
    b) productivo, GSM
    Todo lo recibido se retransmite por I2C al modulo principal
-----------------------------------------------------------------------------*/ 
// Librerias utilizadas
#include <DHT.h>
#include <SoftwareSerial.h>
#include <Wire.h>
// Constantes 
#define VERSION "R1f5v2 -> "
#define DBGLVL  3
#define CHN_BT  false
#define CHN_ZIG false
#define CHN_SRL true
#define CHN_I2C true
#include "C:\Users\Juan\Documents\Arduino\DBGmacros\DBGmacros.ino"
#define MI_ID   1       // Identificador del modulo (id=0 para MASTER; id=1 para REMOTO1; ...)
#define WIREDST 0 
//-
#define DhtGND  8       // DHT22 - pin GND 
#define DhtPIN  9       // DHT22 - pin de lectura
#define DhtVCC  10      // DHT22 - pin VCC 
#define DhtTYPE DHT22   // DHT22-  tipo de sensor de temperatura y humedad 
//-
#define btVCC   7       // HC05 - pin VCC
#define btGND   6       // HC05 - pin GND
#define btTX    5       // HC05 - pin TX
#define btRX    4       // HC05 - pin RX
//-
#define releENT 12      // Pin de control del Rele 1 (Entrada)
#define releCLF 13      // Pin de control del Rele 2 (Calefaccion)
// Variables Globales
int            temp, hume;
String         msg;
SoftwareSerial Bt(btTX, btRX); 
DHT            Dht(DhtPIN, DhtTYPE);

void setup(){    // --- SETUP del Remoto1 ---
  // activar pines alimentacion Bluetooth
//  pinMode(btVCC,   OUTPUT);  digitalWrite(btVCC,   HIGH);
//  pinMode(btGND,   OUTPUT);  digitalWrite(btGND,    LOW);
  // activar pines Reles a HIGH (apagados)
  pinMode(releENT, OUTPUT);  digitalWrite(releENT, HIGH);
  pinMode(releCLF, OUTPUT);  digitalWrite(releCLF, HIGH);
  // activar pines DHT22
  pinMode(DhtVCC, OUTPUT);   digitalWrite(DhtVCC,  HIGH);
  pinMode(DhtGND, OUTPUT);   digitalWrite(DhtGND,   LOW);
  // inicializa el DHT22, el Serial y el I2C
  Dht.begin();
  Bt.begin(9600);   //era 57600 
  Serial.begin(9600);
  Wire.begin(MI_ID);            
  Wire.onReceive(leer_I2C);     // Al recibir una petición, ejecuta rutina "leer_I2C"
  DBGLOG_F2("Inicializando Remoto 1",""); 
  delay(2000);
}


void loop() {    // --- LOOP del Remoto1 ---
  if (millis() % 6500 == 0) 
    ejecutar_accion(1,0,0,0);
  if (Serial.available())   
    leer_Serial();  // Permite el envio de MSG-prueba por consola 
  if (Bt.available())       
    leer_Bt_reenviar_I2C();      // Entrada inalambrica de MSGs
}


void leer_Bt_reenviar_I2C() 
    {
    DBGLOG_F2("Entrando en leerBt()","");
    String msg = "";
    char inChar;
    // posicionamiento al principio del mensaje
    while (Bt.available() && inChar != '#' ) 
      {
      inChar = Bt.read();
      DBGLOG_F2("Bt- Leido y desechado: ",inChar);
      }  
    // carga msg hasta el final del mensaje
    while (Bt.available() && inChar != '/' ) 
      {
      inChar = Bt.read();
      msg += inChar;
      }  
    DBGLOG_F2("MSG leido: #",msg);
    escribir_I2C(msg);
    }


