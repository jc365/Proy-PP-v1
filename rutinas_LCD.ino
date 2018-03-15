
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
    //lcd.clear(); analogWrite(A0,200+(j+1)*1); lcd.print(200+(j+1)*1); delay(500);
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
