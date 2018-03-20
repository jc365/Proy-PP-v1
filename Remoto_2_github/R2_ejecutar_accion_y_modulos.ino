void ejecutar_accion( int srv, int p1, int p2, int p3) {  
  //--- Ejecucion de acciones recibidas
  String msg ;
  log_v2(1,"Ejecutando MSG (srv / p1 / p2 / p3): ", srv, p1, p2, p3);
  switch(srv) {
    case 1:
      log_v2(1,"SRV-1   Lectura de sensores",-99,-99,-99,-99);
      //--- lectura de humedad y temperatura
      hume = Dht.readHumidity();      
      temp = Dht.readTemperature();   
      //--- rutina que lee la luz y fija su estado
      Gestionar_luz();
      //--- composicion y contestacion al Principal con lectura tomada: "#destino:srv:temp:hume:luz/" 
      msg = "#0:11:" + String(temp) + ":" + String(hume) + ":" + String(LDR_estado) + "/";
      Zig.print(msg); 
      log_v2(1,"Contestacion al Principal. MSG enviado: "+msg,-99,-99,-99,-99);
      break;
    case 2:
      log_v2(1,"SRV-2   Encender/Apagar releFAR",p1,-99,-99,-99);
      digitalWrite(releFAR, p1); 
      break;  
    case 3:
      log_v2(1,"SRV-3   Encender/Apagar releCST",p1,-99,-99,-99);
      digitalWrite(releCST, p1);  
      break; 
    case 4:
      log_v2(1,"SRV-4   Encender/Apagar releCHM",p1,-99,-99,-99);
      digitalWrite(releCHM, p1);   
      break;
    case 5:
      log_v2(1,"SRV-5   Encender/Apagar releCAR",p1,-99,-99,-99);
      digitalWrite(releCAR, p1);  
      break; 
    case 6:
      log_v2(1,"SRV-6    Encender/Apagar TodosLosReles",p1,-99,-99,-99);
      digitalWrite(releFAR, p1);
      digitalWrite(releCST, p1);
      digitalWrite(releCHM, p1);
      digitalWrite(releCAR, p1);
      break;
    default:
      log_v2(1,"SRV-??   Servicio no definido", srv, p1, p2, p3);
  }
}


void R2_leer_sondas() 
  {
  log_v2(1,"Registro de temp. interior",-99,-99,-99,-99);
  //--- lectura de humedad y temperatura
  hume = Dht.readHumidity();      
  temp = Dht.readTemperature();   
  //--- rutina que lee la luz y fija su estado
  Gestionar_luz();
  //--- composicion y contestacion al Principal con lectura tomada: "#destino:srv:temp:hume:luz/" 
  msg = "#0:11:" + String(temp) + ":" + String(hume) + ":" + String(LDR_estado) + "/";
  Zig.print(msg);  
  log_v2(1,"Notificacion al Principal. MSG enviado: "+msg,-99,-99,-99,-99);
  }


void Gestionar_luz() 
  {
  //--- medicion del sensor de luz LDR
  LDR_lectura = analogRead(LDRPIN);
  //--- fijar el estado (DIA/NOCHE) ---
  if (LDR_lectura > LDR_corte_op) LDR_estado = NOCHE ;
  else                            LDR_estado = DIA   ;
  log_v2(3,"LDR- Fijado estado de la Luz (DIA=0 / NOCHE=-100): ",LDR_estado,-99,-99,-99); 
  //--- gestionar activadores  segun estado del ultimo tratamiento ---
  if (LDR_estado != LDR_ult_tto) 
    {
    log_v2(3,"LDR- Ha cambiado el estado",-99,-99,-99,-99); 
    //--- ajuste de corte para evitar falsos positivos el umbral
    LDR_corte_op = LDR_umbral_luz + LDR_estado ;
    LDR_ult_tto = LDR_estado ;
    }
  }


