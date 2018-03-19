
// Librerias utilizadas
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
// Constantes 
#define VERSION  "Pf5v1 -> "
#define DBGLVL   3
#undef  CHN_BT  
#define CHN_ZIG 
#define CHN_SRL 
#define CHN_I2C 
#define MI_ID    0       // Identificador del modulo (id=0 para MASTER; id=1 para REMOTO1; ...)
#define WIREDST  1       // Id destino de msgs por I2C
//-
#define DIA      0       // Estos valores DIA/NOCHE estan ajustados para que
#define NOCHE    -100    // sumado al umbral fije un margen de tolerancia para evitar encender/apagar por pequeños cambios
//- 
#define ZigVCC   2       // Detalle de los pines ...
#define ZigTX    3       // a los que conectado el ...
#define ZigRX    4       // Remote ZigBee CC2530 ...   
#define ZigGND   5       // 2.4G Serial 
#define ZigVCC   15      // Detalle de los pines ...
#define ZigTX    16      // a los que conectado el ...
#define ZigRX    17      // Remote ZigBee CC2530 ...   
#define ZigGND   18  
//-
#define releENT  12      // Pin de control del Rele 1 (Entrada)
#define releCLF  13      // Pin de control del Rele 2 (Calefaccion)
//-
int     LDR_estado     = NOCHE ;
int     LDR_sw_ent     = "C"   ;    // S=encendido / N=apagado / C=crepuscular
int     LDR_sw_frl     = "S"   ;    // S=encendido / N=apagado / C=crepuscular
int     LDR_ult_tto    = DIA   ;
//-
int     dlt_lectura    = 2000  ;
int     cont_registro  = 0     ;
int     temp_int, temp_ext, hume_int, hume_ext ;
int     tMax           = -90   ;    // Variables para registrar valores minimo y Maximo
int     tMin           = 200   ; 
String  fecha, hora;
//-  diseño del grafico de caracter "grado" (º)
byte grado[8] = {0b00110,0b01001,0b01001,0b00110,0b00000,0b00000,0b00000,0b00000};
//-
SoftwareSerial Zig(ZigTX, ZigRX);
LiquidCrystal  lcd(12, 11, 5, 4, 3, 2);


void setup() {
  // activar pines de alimentacion
  pinMode(ZigVCC, OUTPUT);  digitalWrite(ZigVCC, HIGH);
  pinMode(ZigGND, OUTPUT);  digitalWrite(ZigGND, LOW);
  Zig.begin(57600);
  Serial.begin(9600);
  Wire.begin(MI_ID);            
  Wire.onReceive(leer_I2C);     // Al recibir una petición, ejecuta rutina "leer_I2C"
  log_v2(1,"Arranque de la placa PRINCIPAL...",-99,-99,-99,-99);  
}  

void loop() 
  {
  Medir_Sondas_Locales(dlt_lectura);
  delay(1600);                            // Calculado por tanteo
  if (Serial.available()) leer_Serial();  // Permite el envio de MSG-prueba por consola 
  if (Zig.available())    leer_Zig();     // Salida/Contestacion de MSGs con remotos
  }



