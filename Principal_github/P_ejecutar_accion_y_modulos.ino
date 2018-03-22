void ejecutar_accion( int srv, int p1, int p2, int p3) {  
  // Ejecucion de acciones recibidas
  log_v2(2,"Ejecutando MSG (srv / p1 / p2 / p3): ", srv, p1, p2, p3);
  switch(srv) 
  { 
    case 10:
      log_v2(1,"SRV-1   Set de umbral de NOCHE: ",p1,-99,-99,-99);
      Zig.print("#2:9:" + String(p1) + ":0:0/"); 
      break;
    case 11:
      log_v2(1,"SRV-11  Registro temp. exterior y luz (temp / hume / luz): ",p1,p2,p3,-99);
      hume_ext = p2 ;
      temp_ext = p1 ;
      // Registrar valores máximos y minimos
      if (temp_ext > tMax)    tMax = temp_ext; 
      if (temp_ext < tMin)    tMin = temp_ext; 
      log_v2(2,"Temp extremas EXTERIOR (min / MAX): ",tMin,tMax,-99,-99);
      logica_luz(p3) ; 
      break;  
    case 12:
      log_v2(1,"SRV-12  Registro temp. interior (temp / hume ): ",p1,p2,-99,-99);
      temp_int = p1 ;
      hume_int = p2 ;
      break; 
    case 13:
      log_v2(1,"SRV-13  Set de FECHA (dd / mm / aaaa): ",p1,p2,p3,-99);
      setTime(hour(),minute(),second(),p1,p2,p3); 
      break;  
    case 14:
      log_v2(1,"SRV-14  Set de HORA (hh / mm / ss): ",p1,p2,p3,-99);
      setTime(p1,p2,p3,day(),month(),year()); 
      break; 
    case 20:
      log_v2(1,"SRV-20  Encender/Apagar releFAR: ",p1,-99,-99,-99);
      Zig.print("#2:2:" + String(p1) + ":0:0/"); 
      break;
    case 30:
      log_v2(1,"SRV-30  Encender/Apagar releCST: ",p1,-99,-99,-99);
      Zig.print("#2:3:" + String(p1) + ":0:0/");  
      break;
    case 40:
      log_v2(1,"SRV-40  Encender/Apagar releCHM: ",p1,-99,-99,-99);
      Zig.print("#2:4:" + String(p1) + ":0:0/");   
      break;
    case 50:
      log_v2(1,"SRV-50  Encender/Apagar releCRV: ",p1,-99,-99,-99);
      Zig.print("#2:5:" + String(p1) + ":0:0/");   
      break;
    case 60:
      log_v2(1,"SRV-60  Encender/Apagar todos los reles (los 6): ",p1,-99,-99,-99);
      escribir_I2C("#1:6:" + String(p1) + ":0:0/");   
      Zig.print("#2:6:" + String(p1) + ":0:0/"); 
      break;
    case 70:
      log_v2(1,"SRV-70  Encender/Apagar releENT: ",p1,-99,-99,-99);
      escribir_I2C("#1:7:" + String(p1) + ":0:0/");    
      break;
    case 80:
      log_v2(1,"SRV-80  Encender/Apagar releCLF: ",p1,-99,-99,-99);
      escribir_I2C("#1:8:" + String(p1) + ":0:0/");    
      break;  
    default:
       log_v2(1,"SRV-xx  Servicio no definido: ", srv, p1, p2, p3);
       break;
  }
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


void logica_luz(int lectura_luz) 
  {
  switch(lectura_luz) 
    {
    case DIA:
      log_v2(3,"Ahora es de DIA",-99,-99,-99,-99);
      if (swENT = "C")
        escribir_I2C("#1:7:1:0:0/");
      if (swFRL = "C")
        Zig.print("#2:2:1:0:0/");  
      break;
    case NOCHE:
      log_v2(3,"Ahora es de NOCHE",-99,-99,-99,-99);
      if (swENT = "C")
        escribir_I2C("#1:7:0:0:0/");
      if (swFRL = "C")
        Zig.print("#2:2:0:0:0/");  
      break;
    default:
      log_v2(1,"Logica_luz(). No es de DIA ni de NOCHE",-99,-99,-99,-99);
    }
  }

