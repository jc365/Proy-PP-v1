void ejecutar_accion( int srv, int p1, int p2, int p3) {  
  // Ejecucion de acciones recibidas
  DBGLOG_F5("Ejecutando MSG (srv / p1 / p2 / p3): ", srv, p1, p2, p3);
  switch(srv) 
  { 
    case 10:
      DBGLOG_F5("SRV-1   Set de umbral de NOCHE: ",p1,-99,-99,-99);
      Zig.print("#2:9:" + String(p1) + ":0:0/"); 
      break;
    case 11:
      DBGLOG_F5("SRV-11  Registro temp. exterior y luz (temp / hume / luz): ",p1,p2,p3,-99);
      hume_ext = p2 ;
      temp_ext = p1 ;
      // Registrar valores máximos y minimos
      if (temp_ext > tMax)    tMax = temp_ext; 
      if (temp_ext < tMin)    tMin = temp_ext; 
      DBGLOG_F5("Temp extremas EXTERIOR (min / MAX): ",tMin,tMax,-99,-99);
      luz = p3;
      acciones_luz() ; 
      break;  
    case 12:
      DBGLOG_F5("SRV-12  Registro temp. interior (temp / hume ): ",p1,p2,-99,-99);
      temp_int = p1 ;
      hume_int = p2 ;
      break; 
    case 13:
      DBGLOG_F5("SRV-13  Set de FECHA (dd / mm / aaaa): ",p1,p2,p3,-99);
      setTime(hour(),minute(),second(),p1,p2,p3); 
      break;  
    case 14:
      DBGLOG_F5("SRV-14  Set de HORA (hh / mm / ss): ",p1,p2,p3,-99);
      setTime(p1,p2,p3,day(),month(),year()); 
      break; 
    case 20:
      DBGLOG_F5("SRV-20  Encender/Apagar releFAR: ",p1,-99,-99,-99);
      Zig.print("#2:2:" + String(p1) + ":0:0/"); 
      break;
    case 30:
      DBGLOG_F5("SRV-30  Encender/Apagar releCST: ",p1,-99,-99,-99);
      Zig.print("#2:3:" + String(p1) + ":0:0/");  
      break;
    case 40:
      DBGLOG_F5("SRV-40  Encender/Apagar releCHM: ",p1,-99,-99,-99);
      Zig.print("#2:4:" + String(p1) + ":0:0/");   
      break;
    case 50:
      DBGLOG_F5("SRV-50  Encender/Apagar releCRV: ",p1,-99,-99,-99);
      Zig.print("#2:5:" + String(p1) + ":0:0/");   
      break;
    case 60:
      DBGLOG_F5("SRV-60  Encender/Apagar todos los reles (los 6): ",p1,-99,-99,-99);
      escribir_I2C("#1:6:" + String(p1) + ":0:0/");   
      Zig.print("#2:6:" + String(p1) + ":0:0/"); 
      break;
    case 70:
      DBGLOG_F5("SRV-70  Encender/Apagar releENT: ",p1,-99,-99,-99);
      escribir_I2C("#1:7:" + String(p1) + ":0:0/");    
      break;
    case 80:
      DBGLOG_F5("SRV-80  Encender/Apagar releCLF: ",p1,-99,-99,-99);
      escribir_I2C("#1:8:" + String(p1) + ":0:0/");    
      break;  
    default:
       DBGLOG_F5("SRV-xx  Servicio no definido: ", srv, p1, p2, p3);
       break;
  }
}

void Mostrar_display_1() {
  String msg[2];
  msg[0] = FECHA_HORA_Y2 + ( (luz == DIA) ? " D" : " N" );
  msg[1] = "Int: " + String(temp_int) + "@ / " + String(hume_int) + "%" ; 
  int p = String(temp_int).length()+5;
  lcd.clear() ; 
  lcd.setCursor(0, 0) ; lcd.print(msg[0]);
  lcd.setCursor(0, 1) ; lcd.print(msg[1]);
  lcd.setCursor(p, 1) ; lcd.write((byte)0); // sobreescribe la @ por el simbolo del grado
  do {
  } while (digitalRead(botROJO) == LOW);
}

void Mostrar_display_2() {
  String msg[2];
  msg[0] = "Ext: " + String(temp_ext) + "@ / " + String(hume_ext) + "%" ;
  msg[1] = "@min/MAX: " + String(tMin) + "/" + String(tMax) ;
  int q = String(temp_ext).length()+5;
  lcd.clear() ; 
  lcd.setCursor(0, 0) ; lcd.print(msg[0]);
  lcd.setCursor(q, 0) ; lcd.write((byte)0); // sobreescribe la @ por el simbolo del grado
  lcd.setCursor(0, 1) ; lcd.print(msg[1]);
  lcd.setCursor(0, 1) ; lcd.write((byte)0); // sobreescribe la @ por el simbolo del grado
  do {
  } while (digitalRead(botROJO) == LOW);
}

void Mostrar_display_3() {
  String msg[2];
  msg[0] = "ENTRADA: " + swENT ;
  msg[1] = "  FAROLAS: " + swFRL ;
  lcd.clear() ; 
  lcd.setCursor(0, 0) ; lcd.print(msg[0]);
  lcd.setCursor(0, 1) ; lcd.print(msg[1]);
  do {
  } while (digitalRead(botROJO) == LOW);
}

void Mostrar_display_4() {
  String msg[2];
  msg[0] = "CST:" + swCST + "   CHM:" + swCHM ;
  msg[1] = "  CRV:" + swCRV + "   CLF:" + swCLF ;
  lcd.clear() ; 
  lcd.setCursor(0, 0) ; lcd.print(msg[0]);
  lcd.setCursor(0, 1) ; lcd.print(msg[1]);
  do {
  } while (digitalRead(botROJO) == LOW);
}

void acciones_luz() {
  switch(luz) {
  case DIA:
    DBGLOG_F2("Logica_luz(). Ahora es de DIA","");
    if (swENT == "l") escribir_I2C("#1:7:1:0:0/");
    if (swFRL == "l")    Zig.print("#2:2:1:0:0/");  
    break;
  case NOCHE:
    DBGLOG_F2("Logica_luz(). Ahora es de NOCHE","");
    if (swENT == "l") escribir_I2C("#1:7:0:0:0/");
    if (swFRL == "l")    Zig.print("#2:2:0:0:0/");  
    break;
  default:
    DBGLOG_F2("Logica_luz(). No es de DIA ni de NOCHE","");
  }
}


String Configurar_sw(String txt, String sw, byte n_opc) {
  byte   n ;
  byte   a_pos[] = { 2 , 7 , 12} ;
  String a_opc[] = {"s","n","l"} ;
  
  for (n = 2 ; n >= 0 ; n--) { if (sw == a_opc[n]) break; }
  DBGLOG_F2("Entra a Configurar_sw(). Invocado con: ",txt + " / " + sw);

  lcd.clear() ;
  digitalWrite(lcdPOWER,   HIGH);  
  lcd.setCursor(       0, 0) ; lcd.print(txt);
  lcd.setCursor(       0, 1) ; lcd.print(( (n_opc == 3) ? "   si   no   luz" : "   si   no " ));;
  lcd.setCursor(a_pos[n], 1) ; lcd.write(">"); 
  
  do {  
    DBGLOG_F2("Dentro del bucle de espera para soltar los 2 botones","");
  } while (!(digitalRead(botROJO) == HIGH && digitalRead(botAMAR) == HIGH));
  DBGLOG_F2("Ya estan soltados los 2 botones.","");
  
  for (;;) {
    if (digitalRead(botROJO) == LOW && digitalRead(botAMAR) == LOW) {
      DBGLOG_F2("Saliendo con los 2 botones pulsados","");
      return "z";
      
    } else if (digitalRead(botROJO) == LOW) {
      return a_opc[n];
      
    } else if (digitalRead(botAMAR) == LOW) {
      do {
        lcd.setCursor(a_pos[n], 1) ; lcd.write(" ");  //- deselecciona la opcion anterior
        n++;  if (n == n_opc) n = 0 ;                  //- si al incrementar se sale del rango, pone a 0
        lcd.setCursor(a_pos[n], 1) ; lcd.write(">");  //- selecciona la nueva opcion
        DBGLOG_F5("Pulsado el amarillo. Valores (despues) de n / pos: ",n,a_pos[n],-99,-99);
        delay(200);
      } while (digitalRead(botAMAR) == LOW);
    }
  }  
}
