/*----------------------------------------------------------------------------
  PROGRAMACION DEL REMOTO 1 (instado en caseta)
  
  Sensores / actuadores conectados:
  - Sensor de temperatura DHT22
  - Modulo de transmision Remote ZigBee CC2530 2.4G Serial 
    configurado en modo Broadcast, canal 1, velocidad 9600
  - 4 reles 
  - Sensor de luz, en puente con resistencias de xx ohmios
-----------------------------------------------------------------------------*/ 
// Librerias utilizadas
#include <DHT.h>
#include <SoftwareSerial.h>
// Constantes 
#define VERSION "R1F3v1-> "
#define DhtGND  8       // Pin GND del DHT22
#define DhtPIN  9       // Pin de lectura del DHT22
#define DhtVCC  10      // Pin VCC del DHT22
#define DhtTYPE DHT22   // Tipo de sensor de temperatura y humedad (DHT22)  
#define ZigVCC  2       // Detalle de los pines ...
#define ZigTX   3       // a los que conectado el ...
#define ZigRX   4       // Remote ZigBee CC2530 ...   
#define ZigGND  5       // 2.4G Serial 
#define releFAR 12       // Pin de control del Rele 1 (Farolas)
#define releCST 13       // Pin de control del Rele 2 (Caseta)
#define releCHM 6      // Pin de control del Rele 3 (Chamizo)
#define releCAR 7      // Pin de control del Rele 4 (Caravana)
#define MI_ID   1       // Identificador del modulo ZigBee (id=0 para MASTER; id=1 para REMOTO1)
// Variables Globales
SoftwareSerial Zig(ZigTX, ZigRX); 
DHT            Dht(DhtPIN, DhtTYPE);
int            OnOff = HIGH;
String         msg;

void setup(){
  // --- SETUP del Remoto1 ---
  // activar pines Reles a HIGH (apagados)
  pinMode(releFAR, OUTPUT);  digitalWrite(releFAR, OnOff);
  pinMode(releCST, OUTPUT);  digitalWrite(releCST, OnOff);
  pinMode(releCHM, OUTPUT);  digitalWrite(releCHM, OnOff);
  pinMode(releCAR, OUTPUT);  digitalWrite(releCAR, OnOff);
  // activar pines ZigBee
  pinMode(ZigVCC, OUTPUT);   digitalWrite(ZigVCC,   HIGH);  
  pinMode(ZigGND, OUTPUT);   digitalWrite(ZigGND,    LOW);
  // activar pines DHT22
  pinMode(DhtVCC, OUTPUT);   digitalWrite(DhtVCC,   HIGH);
  pinMode(DhtGND, OUTPUT);   digitalWrite(DhtGND,    LOW);
    // Inicializa el DHT22, el ZigBee y el Serial
  Dht.begin();  Zig.begin(9600);  Serial.begin(9600);
  log("Inicializando sensor y Zigee, espera por favor (valores de OnOff y !OnOff) ...",OnOff,!OnOff,-99,-99); 
  delay(2000);
}

void loop() {
  delay(3000);
  if (Serial.available()) leer_Serial();
  if (Zig.available())    leer_Zig();
  log("------------------",-99,-99,-99,-99);
}

void leer_Serial() {
  log(" entrando en leerSerial",-99,-99,-99,-99);
  msg = "";
  char inChar;
  // posicionamiento al principio del mensaje
  while (Serial.available() && inChar != '#' ) {
    inChar = Serial.read();
    log("  Serial. Leido y desechado: "+inChar,-99,-99,-99,-99);
  }  
  // carga msg hasta el final del mensaje
  while (Serial.available() && inChar != '/' ) {
    inChar = Serial.read();
    msg += inChar;
  }  
  log(" string leido hasta el /: "+msg,-99,-99,-99,-99);
  formatear_msg(msg);
}

void formatear_msg(String in) {
  int n, id, srv, parm1, parm2, parm3;
  char *buf = in.c_str();  // copia de la direccion de msg
  n = sscanf(buf, "%d:%d:%d:%d:%d/", &id, &srv, &parm1, &parm2, &parm3);
  log("   formateo realizado leido (id/srv/p1/p2/p3):", id, srv, parm1, parm2);
  if (n == 5) {
    log("    msg correctamente leido (5 variables parseadas)",-99,-99,-99,-99); 
    if (id == MI_ID) {
      log("      mensaje para tratar, soy el id de destino ",-99,-99,-99,-99); 
      tratar_msg(srv, parm1, parm2, parm3);  
    }
  }  
}

void Medir_Sondas_Locales() {
  //Lectura del sensor DHT22
  int hume = Dht.readHumidity();
  int temp = Dht.readTemperature();
  // Composicion msg al Master con lectura tomada: "#destino:srv:temp:hume:luz/"
  msg = "#0:1:" + String(temp) + ":" + String(hume) + ":0/";
  log(" msg generado en la lectura de temperatura>>> "+msg,-99,-99,-99,-99);
  Zig.print(msg);
}

void leer_Zig() {
  log(" entrando en leerZig",-99,-99,-99,-99);
  msg = "";
  char inChar;
  // posicionamiento al principio del mensaje
  while (Zig.available() && inChar != '#' ) {
    inChar = Zig.read();
    log("  Zig. Leido y desechado: "+inChar,-99,-99,-99,-99);
  }  
  // carga msg hasta el final del mensaje
  while (Zig.available() && inChar != '/' ) {
    inChar = Zig.read();
    msg += inChar;
  }  
  log(" string leido hasta el /: "+msg,-99,-99,-99,-99);
  formatear_msg(msg);
}

void tratar_msg( int srv, int p1, int p2, int p3) {  
  // tratamiento de peticiones recibidas
  log("Tratando msg (srv/p1/p2/p3): ", srv, p1, p2, p3);
  if (srv == 1) {
    log(" Ejecutando servicio medir_sondas ",-99,-99,-99,-99);
    Medir_Sondas_Locales();
  }
  else if (srv == 2) {
    log("ejecutando servicio Encender/Apagar releFAR", srv, p1, p2, p3);
    digitalWrite(releFAR, p1);   
  }
  else if (srv == 3) {
    log("ejecutando servicio Encender/Apagar releCST", srv, p1, p2, p3);
    digitalWrite(releCST, p1);   
  }
  else if (srv == 4) {
    log("ejecutando servicio elseif", srv, p1, p2, p3);
    OnOff = !OnOff;
    digitalWrite(releFAR, OnOff);
    digitalWrite(releCST, !OnOff);    
  }
  else {
    log("ejecutando servicio else", srv, p1, p2, p3);
    Serial.println("vivo");
  }
}
     
void log(String e1, int e2, int e3, int e4, int e5){
  // sw productivo: return;
  // rutina para displayar en modo depuracion; no hace nada en modo produccion
  String s2, s3, s4, s5 ;
  if (e2 == -99) s2 = ""; else s2 = String(e2) + "/";
  if (e3 == -99) s3 = ""; else s3 = String(e3) + "/";
  if (e4 == -99) s4 = ""; else s4 = String(e4) + "/";
  if (e5 == -99) s5 = ""; else s5 = String(e5) + "/";
  Serial.print(VERSION + e1 + s2 + s3 + s4 + s5 + "\n");
}


void leer_Serial_old() {
  log(" entrando en leerSerial",-99,-99,-99,-99);
  char inChar;
  String string = "";
  while (Serial.available()) {
    inChar = Serial.read();
    string += inChar;
  }  
  msg = string;
  log(" leido del serial: " + string,-99,-99,-99,-99);  
  int n, id, srv, parm1, parm2, parm3;
  char *buf = msg.c_str();  // copia de la direccion de msg
  n = sscanf(buf, "#%d:%d:%d:%d:%d/", &id, &srv, &parm1, &parm2, &parm3);
  log("   msg leido:", id, srv, parm1, parm2);
  if (n == 5) {
    log("    msg correctamente leido (5 variables parseadas)",-99,-99,-99,-99); 
    if (id == MI_ID) {
      log("      mensaje para mi ",-99,-99,-99,-99); 
      tratar_msg(srv, parm1, parm2, parm3);  
    }
  }  
}

 



