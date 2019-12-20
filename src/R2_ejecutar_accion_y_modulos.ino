void ejecutar_accion( int srv, int p1, int p2, int p3) {  
  //--- Ejecucion de acciones recibidas
  String msg ;
  DBGLOG_F5("Ejecutando MSG (srv / p1 / p2 / p3): ", srv, p1, p2, p3);
  switch(srv) {
    case 1:
      DBGLOG_F2("SRV-1   Lectura de sensores","");
      //--- lectura de humedad y temperatura
      hume = Dht.readHumidity();      
      temp = Dht.readTemperature();   
      //--- rutina que lee la luz y fija su estado
      Gestionar_luz();
      //--- composicion y contestacion al Principal con lectura tomada: "#destino:srv:temp:hume:luz/" 
      msg = "#0:11:" + String(temp) + ":" + String(hume) + ":" + String(LDR_estado) + "/";
      Zig.print(msg); 
      DBGLOG_F2("Contestacion al Principal. MSG enviado: ",msg);
      break;
    case 2:
      DBGLOG_F5("SRV-2   Encender/Apagar releFAR",p1,-99,-99,-99);
      digitalWrite(releFAR, p1); 
      break;  
    case 3:
      DBGLOG_F5("SRV-3   Encender/Apagar releCST",p1,-99,-99,-99);
      digitalWrite(releCST, p1);  
      break; 
    case 4:
      DBGLOG_F5("SRV-4   Encender/Apagar releCHM",p1,-99,-99,-99);
      digitalWrite(releCHM, p1);   
      break;
    case 5:
      DBGLOG_F5("SRV-5   Encender/Apagar releCAR",p1,-99,-99,-99);
      digitalWrite(releCAR, p1);  
      break; 
    case 6:
      DBGLOG_F5("SRV-6    Encender/Apagar TodosLosReles",p1,-99,-99,-99);
      digitalWrite(releFAR, p1);
      digitalWrite(releCST, p1);
      digitalWrite(releCHM, p1);
      digitalWrite(releCAR, p1);
      break;
    case 9:
      DBGLOG_F5("SRV-9    Registro de umbral de NOCHE: ",p1,-99,-99,-99);
      LDR_umbral_luz = p1;
      break;  
    default:
      DBGLOG_F5("SRV-??   Servicio no definido", srv, p1, p2, p3);
  }
}


//void R2_leer_sondas() 
//  {
//  log_v2(1,"Registro de temp. interior",-99,-99,-99,-99);
//  //--- lectura de humedad y temperatura
//  hume = Dht.readHumidity();      
//  temp = Dht.readTemperature();   
//  //--- rutina que lee la luz y fija su estado
//  Gestionar_luz();
//  //--- composicion y contestacion al Principal con lectura tomada: "#destino:srv:temp:hume:luz/" 
//  msg = "#0:11:" + String(temp) + ":" + String(hume) + ":" + String(LDR_estado) + "/";
//  Zig.print(msg);  
//  log_v2(1,"Notificacion al Principal. MSG enviado: "+msg,-99,-99,-99,-99);
//  }


void Gestionar_luz() 
  {
  //--- medicion del sensor de luz LDR ---
  LDR_lectura = analogRead(LDRPIN);
  //--- fijar el estado (DIA/NOCHE) ---
  LDR_estado = ( (LDR_lectura > LDR_corte_op) ? NOCHE : DIA );
  DBGLOG_F5("LDR- Fijado estado de la Luz (DIA=0 / NOCHE=-100): ",LDR_estado,-99,-99,-99); 
  if (LDR_estado != LDR_ult_tto) 
    {
    DBGLOG_F2("LDR- Ha cambiado el estado",""); 
    //--- ajuste del corte, para evitar falsos positivos durante el umbral
    LDR_corte_op = LDR_umbral_luz + LDR_estado ;
    LDR_ult_tto = LDR_estado ;
    }
  }


