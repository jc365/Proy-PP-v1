#include <SoftwareSerial.h>

//PARA ENTRAR EN MODO ZAT CONECTAR EL ARD-PIN3 CON HC05-KEY
//NAME=HC05 ; ROLE=0 ; PSWD=1234

//#define ZigVCC 2
//#define ZigTX  3
//#define ZigRX  4
//#define ZigGND 5

#define btVCC  7
#define btTX   8
#define btRX   9

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
#define MI_ID   0       // Identificador del modulo ZigBee (id=0 para MASTER; id=1 para REMOTO1)




SoftwareSerial Zig(ZigTX, ZigRX)    ;
String str,  srv, temp, hume;
int a = 0;


//SoftwareSerial Bt(btTX, btRX); // RX | TX

void setup() {
pinMode(btVCC, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
digitalWrite(btVCC, HIGH);

  // activar alimentaiones

  pinMode(ZigVCC, OUTPUT);  // patilla Vcc del Zig
  digitalWrite(ZigVCC, HIGH);
  pinMode(ZigGND, OUTPUT);  // patilla - del Zig
  digitalWrite(ZigGND, LOW);
  Zig.begin(9600);
  //Zig.setTimeout(50);
delay(2000);
Serial.begin(9600);
Serial.println("Testo recepcion en master:");
//Bt.begin(38400);  // HC-05 default speed in AT command more
}

void loop() {
  delay(3000);
  if (Serial.available()) leer_Serial();  // Permite el envio de MSG-prueba por consola 
  if (Zig.available())    leer_Zig();     // Entrada productiva de MSGs
  Zig.print("#1:1:0:0:0/");
}




/* codigo copiado literal
 *  
 */
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
  // codigo alterado
  //formatear_msg(msg);
  Zig.print("#"+msg);
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
//    int hume = Dht.readHumidity();      // lectura de humedad
//    int temp = Dht.readTemperature();   // lectura de temperatura
//    int luz = 300;
//    // Composicion y contestacion al Master con lectura tomada: "#destino:srv:temp:hume:luz/" 
//    msg = "#0:20:" + String(temp) + ":" + String(hume) + ":" + String(luz) + "/";
//    Zig.print(msg); 
//    log(" Contestacion al Master. MSG enviado: "+msg,-99,-99,-99,-99);
  }
  else if (srv == 20) {
    log("      SRV-20  falta añadir logica de recepcion de lecturas",-99,-99,-99,-99);
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
