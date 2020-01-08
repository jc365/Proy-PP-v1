#include <SoftwareSerial.h>

//PARA ENTRAR EN MODO ZAT CONECTAR EL ARD-PIN3 CON HC05-KEY
//NAME=HC05 ; ROLE=0 ; PSWD=1234

#define ZigVCC 2
#define ZigTX  3
#define ZigRX  4
#define ZigGND 5

#define btVCC  7
#define btTX   8
#define btRX   9



SoftwareSerial Zig(ZigTX, ZigRX)    ;
String str,  srv, temp, hume;
int a = 0;


//SoftwareSerial Bt(btTX, btRX); // RX | TX

void setup()
{
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

void loop()
{
//if (Zig.available()) {
//  //Serial.println("ss");
//  Serial.write(Zig.read());
//}

  delay(3000);
  Zig.print("#1:1:0:0:0/");
  Serial.println("acabo de escribir en zig");

 if (Zig.available()) {
   str = Zig.readStringUntil('/');
   Serial.println(str);
   if ( str == "0") {
     tratar_msg();
   }
   else {
     Serial.println("entrando por else");
     str = Zig.readStringUntil('F');
     Zig.flush();
     Serial.println("desechando: ");
     Serial.println(str);
   }
 }
 Zig.println("1/2/-/-F");
}

void tratar_msg() {
      Serial.println("entrando por tratar");
      srv  = Zig.readStringUntil('/');
      temp = Zig.readStringUntil('/'); 
      hume = Zig.readStringUntil('F');  
      int flas = srv.toInt();
      a = a + flas * 2 ;
      Serial.println(srv + "&" + temp + "&" + hume);
      Serial.println(a);
}

