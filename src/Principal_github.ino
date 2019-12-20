// Librerias utilizadas
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>

// Constantes 
#define VERSION  "Pf5v1 -> "
#define DBGLVL   3
#define MI_ID    0       // Identificador del modulo (id=0 para MASTER; id=1 para REMOTO1; ...)
#define WIREDST  1       // Id destino de msgs por I2C
#define CHN_BT   false
#define CHN_ZIG  true
#define CHN_SRL  true
#define CHN_I2C  true
#include "C:\Users\Juan\Documents\Arduino\DBGmacros\DBGmacros.ino"
//-
#define DIA      0       // Estos valores DIA/NOCHE estan ajustados para que sumado al umbral, fije ...
#define NOCHE    -100    // ... un margen de tolerancia para evitar encender/apagar por pequeños cambios
//- 
#define ZigVCC           // Detalle de los pines a ...
#define ZigTX    8       // .. los que esta conectado el ...
#define ZigRX    9       // .. Remote ZigBee CC2530 ...   
#define ZigGND           // .. 2.4G Serial 
//-
#define releENT  12      // Pin de control del Rele 1 (Entrada)
#define releCLF  13      // Pin de control del Rele 2 (Calefaccion)
//-                      //-- DISPLAY 16x2 --- conexiones:
#define lcdRS      11    //* LCD RS pin to digital pin 12
#define lcdENABLE  10    //* LCD Enable pin to digital pin 11
#define lcdD4      5     //* LCD D4 pin to digital pin 5
#define lcdD5      4     //* LCD D5 pin to digital pin 4
#define lcdD6      3     //* LCD D6 pin to digital pin 3
#define lcdD7      2     //* LCD D7 pin to digital pin 2
//---
#define botAMAR   12
#define botROJO   13  
#define lcdPOWER   6
// Variables
String  swENT          = "l"   ;    // S=encendido / N=apagado / L=luz
String  swFRL          = "s"   ;    // S=encendido / N=apagado / L=luz
String  swCLF          = "s"   ;    // S=encendido / N=apagado
String  swCST          = "n"   ;    // S=encendido / N=apagado
String  swCHM          = "n"   ;    // S=encendido / N=apagado
String  swCRV          = "n"   ;    // S=encendido / N=apagado
String  RC                     ;
int     temp_int       = 0     ;
int     temp_ext       = 0     ; 
int     hume_int       = 0     ;
int     hume_ext       = 0     ;
int     luz            = DIA   ;
int     tMax           = -90   ;    // Variables para registrar valores 
int     tMin           = 200   ;    // minimo y Maximo de temp.exterior
String  fecha,  hora;
byte    contador;
//-  diseño grafico de caracter "grado" para el LCD (º)
byte    grado[8]       = {0b00110,0b01001,0b01001,0b00110,0b00000,0b00000,0b00000,0b00000} ;
//-  Instancias
SoftwareSerial Zig(ZigTX, ZigRX);
LiquidCrystal  lcd(lcdRS, lcdENABLE, lcdD4, lcdD5, lcdD6, lcdD7);


void setup() {
  // activar pines de alimentacion
  pinMode(lcdPOWER, OUTPUT); digitalWrite(lcdPOWER,   LOW);
  pinMode(botAMAR, INPUT);
  pinMode(botROJO, INPUT);
  Zig.begin(57600);
  Serial.begin(9600);
  Wire.begin(MI_ID);            
  Wire.onReceive(leer_I2C);     // Al recibir una petición, ejecuta rutina "leer_I2C"
  //setTime(17,35,12,14,1,2018); 
  // Inicializa el LCD y crea un nuevo caracter
  lcd.begin(16, 2);
  lcd.createChar(0, grado);
  DBGLOG_F2("Iniciando estacion PRINCIPAL. Fecha-hora inicial: ",FECHA_HORA_Y4);
}  


void loop() {
  contador = 0;
  delay(1600);                                        // Calculado por tanteo
  if (Serial.available())          leer_Serial();     // Permite el envio de MSG-prueba por consola 
  if (Zig.available())             leer_Zig();        // Salida/Recepcion de MSGs con remotos 
  //if (millis() % 6000 == 0)        Mostrar_display();
  if (digitalRead(botAMAR) == LOW) contador++;
  if (digitalRead(botROJO) == LOW) {
    contador++;
    if (contador == 2) {
      for (;;) {
        RC    = Configurar_sw("Entrada (ENT):" ,swENT,3);       
        if (RC == "z") break;  else swENT = RC ;
        DBGLOG_F2("Valor de ENT seleccionado: ",swENT);   
        
        RC    = Configurar_sw("Farolas (FRL):" ,swFRL,3);
        if (RC == "z") break;  else swFRL = RC ;
        DBGLOG_F2("Valor de FRL seleccionado: ",swFRL); 
        
        RC    = Configurar_sw("Caseta (CST):"  ,swCST,2);
        if (RC == "z") break;  else swCST = RC ;
        DBGLOG_F2("Valor de CST seleccionado: ",swCST);
        
        RC    = Configurar_sw("Chamizo (CHM):"  ,swCHM,2);
        if (RC == "z") break;  else swCHM = RC ;
        DBGLOG_F2("Valor de CHM seleccionado: " ,swCHM);        
        
        RC    = Configurar_sw("Caravana (CRV):" ,swCRV,2);
        if (RC == "z") break;  else swCRV = RC ;
        DBGLOG_F2("Valor de CRV seleccionado: " ,swCRV);
        
        RC    = Configurar_sw("Calefac. (CLF):" ,swCLF,2);
        if (RC == "z") break;  else swCLF = RC ;
        DBGLOG_F2("Valor de CLF seleccionado: " ,swCLF);
        break;
      }
      lcd.clear() ;
      digitalWrite(lcdPOWER,   LOW);
    } else {
      digitalWrite(lcdPOWER,  HIGH);
      Mostrar_display_1();
      delay(2000);
      if (digitalRead(botROJO) == LOW) {
        Mostrar_display_2();
        delay(2000);
        if (digitalRead(botROJO) == LOW)  {
          Mostrar_display_3();
          delay(2000);
          if (digitalRead(botROJO) == LOW)  {
            Mostrar_display_4();
            delay(2000);
          }
        }
      }
      lcd.clear() ;
      digitalWrite(lcdPOWER,   LOW);  
    }
  } 
}


