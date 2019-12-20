#include <DHT.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>

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
 
void setup(){
  // Inicializa el LCD, el sensor DHT22 y el Serial
  lcd.begin(16, 2);
  Serial.begin(9600);
  dht.begin();
  // Ajuste de hora a mano. Pone: 14/1/2018 17:35:12
  setTime(17,35,12,14,1,2018); 
  // create a new character
  lcd.createChar(0, grado);
}

// Código loop del MASTER
void loop() {
  Medir_Sondas_Locales(dlt_lectura);
  delay(dlt_lectura);
  Mostrar_display();
}

void Medir_Sondas_Locales(int n) {
  //Lectura del sensor de temp y humedad DHT22
  hume_int = dht.readHumidity();
  temp_int = dht.readTemperature();
  cont_registro += n ;
  Serial.println("RELLENAR: Solicitar Medir_Sondas_Locales al Esclavo1");
  Serial.print("Cont_registro: " + String(cont_registro) + "\t");
  Serial.print("Temp_int: " + String(temp_int) + " º\t");
  Serial.println("Humd_int: " + String(hume_int) + " %\t");

  //La siguiente funcion se debe ejecutar al recibir las mediciones remotas. SIMULADO
  Anotar_Lecturas_Remotas(15, 67);
}

void Anotar_Lecturas_Remotas(int t, int h) {
  temp_ext = t;
  hume_ext = h;  

   // Registrar valores máximos y minimos
  if (temp_ext > tMax) {
    tMax = temp_ext;
  }
  if (temp_ext < tMin) {
    tMin = temp_ext;
  }

  // Envia el dato al puerto serial
  Serial.print("Temp/Humd ext: " + String(temp_ext) + "º/" + String(hume_ext) + "%\n");
  Serial.print("Temperatura min/MAX: " + String(tMin) + "º/" + String(tMax) + "º\n");
  Serial.print("Intervalos de lectura y de registro: " + String(dlt_lectura) + String(cont_registro) + "%\n");
  Serial.print("Fecha:" + fecha + " Hora: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  // println ya incluye retorno de carro y de linea
  Serial.println(second()); 
  Serial.print("\n");
}
 
void Mostrar_display() {
  temp_ext = -14; //arreglar
  String msg[4];
  msg[0] = RellDig(day())+"/"+RellDig(month())+"/"+String(year()) + " " + RellDig(hour())+":"+RellDig(minute());
  msg[0].remove(6,2); // quita los 2 primeros digitos del año
  msg[0] = msg[0] + " n"; //arreglar con dia/noche d/n
  msg[1] = "Int: " + String(temp_int) + "@ / " + String(hume_int) + "%" ; 
  msg[2] = "Ext: " + String(temp_ext) + "@ / " + String(hume_ext) + "%" ;
  msg[3] = "@min/MAX: " + String(tMin) + "/" + String(tMax) ;
  int p = String(temp_int).length()+5;
  int q = String(temp_ext).length()+5;
  //lcd.setCursor(msg[1].lastIndexOf("@"), 1) ; lcd.write((byte)0); // es otra forma de hacerlo, pero el pgm es + grande
  for (int j=0; j<3; j++) {
    lcd.clear() ; 
    lcd.setCursor(0, 0) ; lcd.print(msg[0]);
    lcd.setCursor(0, 1) ; lcd.print(msg[1]);
    lcd.setCursor(p, 1) ; lcd.write((byte)0); // sobreescribe la @ por el simbolo del grado
    delay(2500);
    lcd.clear() ; 
    lcd.setCursor(0, 0) ; lcd.print(msg[2]);
    lcd.setCursor(q, 0) ; lcd.write((byte)0); // sobreescribe la @ por el simbolo del grado
    lcd.setCursor(0, 1) ; lcd.print(msg[3]);
    lcd.setCursor(0, 1) ; lcd.write((byte)0); // sobreescribe la @ por el simbolo del grado
    delay(3000);
    lcd.clear() ; 
    lcd.setCursor(0, 0) ; lcd.print(msg[0]);
    lcd.setCursor(0, 1) ; lcd.print(msg[1]);
    lcd.setCursor(p, 1) ; lcd.write((byte)0); // sobreescribe la @ por el simbolo del grado
  }
}


String RellDig(int n) {
  if(n < 10) return String("0" + String(n));
  else       return String(n);    
}


