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
#define VERSION "R1F3Vfinal -> "
#define DhtGND  8       // Pin GND del DHT22
#define DhtPIN  9       // Pin de lectura del DHT22
#define DhtVCC  10      // Pin VCC del DHT22
#define DhtTYPE DHT22   // Tipo de sensor de temperatura y humedad (DHT22)  
#define ZigVCC  2       // Detalle de los pines ...
#define ZigTX   3       // a los que conectado el ...
#define ZigRX   4       // Remote ZigBee CC2530 ...   
#define ZigGND  5       // 2.4G Serial 
#define releFAR 12      // Pin de control del Rele 1 (Farolas)
#define releCST 13      // Pin de control del Rele 2 (Caseta)
#define releCHM 6       // Pin de control del Rele 3 (Chamizo)
#define releCAR 7       // Pin de control del Rele 4 (Caravana)
#define MI_ID   1       // Identificador del modulo ZigBee (id=0 para MASTER; id=1 para REMOTO1)
// Variables Globales
SoftwareSerial Zig(ZigTX, ZigRX); 
DHT            Dht(DhtPIN, DhtTYPE);

void setup(){
  // --- SETUP del Remoto1 ---
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
    // Inicializa el DHT22, el ZigBee y el Serial
  Dht.begin();  Zig.begin(9600);  Serial.begin(9600);
  log("Inicializando sensor y Zigee, espera por favor. Valores int de HIGH/LOW: ",HIGH,LOW,-99,-99); 
  delay(2000);
}

void loop() {
  delay(1000);
  if (Serial.available()) leer_Serial();  // Permite el envio de MSG-prueba por consola 
  if (Zig.available())    leer_Zig();     // Entrada productiva de MSGs
  //log("------------------",-99,-99,-99,-99);
}

void leer_Serial() {
  log(" entrando en leerSerial",-99,-99,-99,-99);
  String msg = "";
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
  log(" MSG leido: #"+msg,-99,-99,-99,-99);
  formatear_msg(msg);
}

void leer_Zig() {
  log(" entrando en leerZig",-99,-99,-99,-99);
  String msg = "";
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
  log(" MSG leido: #"+msg,-99,-99,-99,-99);
  formatear_msg(msg);
}

void formatear_msg(String in) {
  int n, id, srv, parm1, parm2, parm3;
  char *buf = in.c_str();  // copia de la direccion de msg
  n = sscanf(buf, "%d:%d:%d:%d:%d/", &id, &srv, &parm1, &parm2, &parm3);
  log("   formateo realizado (srv / p1 / p2 / p3): ", srv, parm1, parm2, parm3);
  if (n == 5) {
    log("     MSG correctamente leido (5 variables parseadas)",-99,-99,-99,-99); 
    if (id == MI_ID) {
      log("      Hay que ejecutar. Soy el id de destino.  ",-99,-99,-99,-99); 
      ejecutar_accion(srv, parm1, parm2, parm3);  
    }
  }  
}

void ejecutar_accion( int srv, int p1, int p2, int p3) {  
  // Ejecucion de acciones recibidas
  String msg ;
  log("Ejecutando MSG (srv / p1 / p2 / p3): ", srv, p1, p2, p3);
  if (srv == 1) {   
    log("      SRV-1  Lectura de sensores",-99,-99,-99,-99);
    int hume = Dht.readHumidity();      // lectura de humedad
    int temp = Dht.readTemperature();   // lectura de temperatura
    int luz = 300;
    // Composicion y contestacion al Master con lectura tomada: "#destino:srv:temp:hume:luz/" 
    msg = "#0:20:" + String(temp) + ":" + String(hume) + ":" + String(luz) + "/";
    Zig.print(msg); 
    log(" Contestacion al Master. MSG enviado: "+msg,-99,-99,-99,-99);
  }
  else if (srv == 2) {
    log("      SRV-2   Encender/Apagar releFAR",-99,-99,-99,-99);
    digitalWrite(releFAR, p1);   
  }
  else if (srv == 3) {
    log("      SRV-3   Encender/Apagar releCST",-99,-99,-99,-99);
    digitalWrite(releCST, p1);   
  }
  else if (srv == 4) {
    log("      SRV-4   Encender/Apagar releCHM",-99,-99,-99,-99);
    digitalWrite(releCHM, p1);   
  }
  else if (srv == 5) {
    log("      SRV-5   Encender/Apagar releCAR",-99,-99,-99,-99);
    digitalWrite(releCAR, p1);   
  }
  else if (srv == 6) {
    log("      SRV-6    Encender/Apagar TodosLosReles",-99,-99,-99,-99);
    digitalWrite(releFAR, p1);
    digitalWrite(releCST, p1);
    digitalWrite(releCHM, p1);
    digitalWrite(releCAR, p1);
  }
  else {
    log("      SRV-??   Servicio no definido", srv, p1, p2, p3);
  }
}
     
void log(String e1, int e2, int e3, int e4, int e5){
  // sw productivo: return;
  // rutina para displayar en modo depuracion; no hace nada en modo produccion
  String s2, s3, s4, s5 ;
  if (e2 == -99) s2 = ""; else s2 = String(e2) + " / ";
  if (e3 == -99) s3 = ""; else s3 = String(e3) + " / ";
  if (e4 == -99) s4 = ""; else s4 = String(e4) + " / ";
  if (e5 == -99) s5 = ""; else s5 = String(e5)        ;
  Serial.print(VERSION + e1 + s2 + s3 + s4 + s5 + "\n");
}


 


