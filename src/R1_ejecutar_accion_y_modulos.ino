void ejecutar_accion( int srv, int p1, int p2, int p3) {  
  //--- Ejecucion de acciones recibidas
  String msg ;
  int temp, hume = 0 ;
  DBGLOG_F5("Ejecutando MSG (srv / p1 / p2 / p3): ", srv, p1, p2, p3);
  switch(srv) {
    case 1:
      DBGLOG_F2("SRV-1   Lectura de sensores","");
      //--- lectura de humedad y temperatura interior
      hume = Dht.readHumidity();      
      temp = Dht.readTemperature();   
      //--- composicion y contestacion al Principal con lectura tomada: "#destino:srv:ttemp:hume:luz/" 
      escribir_I2C("#0:12:" + String(temp) + ":" + String(hume) + ":0/\0"); 
      DBGLOG_F2("Notificacion al Principal. MSG enviado: ",msg);
      break;
    case 6:
      DBGLOG_F5("SRV-6    Encender/Apagar TodosLosReles",p1,-99,-99,-99);
      digitalWrite(releENT, p1);
      digitalWrite(releCLF, p1);
      break;
    case 7:
      DBGLOG_F5("SRV-2   Encender/Apagar releENT",p1,-99,-99,-99);
      digitalWrite(releENT, p1); 
      break;  
    case 8:
      DBGLOG_F5("SRV-3   Encender/Apagar releCLF",p1,-99,-99,-99);
      digitalWrite(releCLF, p1);  
      break; 
    default:
      DBGLOG_F5("SRV-??   Servicio no definido....", srv, p1, p2, p3);
  }
}


//void R1_leer_sondas() 
//  {
//  log_v2(1,"Registro de temp. interior",-99,-99,-99,-99);
//  //--- lectura de humedad y temperatura
//  hume = Dht.readHumidity();      
//  temp = Dht.readTemperature();   
//  //--- composicion y contestacion al Principal con lectura tomada: "#destino:srv:ttemp:hume:luz/" 
//  msg = "#0:12:" + String(temp) + ":" + String(hume) + ":0/\0";
//  escribir_I2C(msg); 
//  log_v2(1,"Notificacion al Principal. MSG enviado: "+msg,-99,-99,-99,-99);
//  }



