void ejecutar_accion( int srv, int p1, int p2, int p3) {  
  // Ejecucion de acciones recibidas
  log_v2(2,"Ejecutando MSG (srv / p1 / p2 / p3): ", srv, p1, p2, p3);
  switch(srv) {
    case 10:
      log_v2(1,"SRV-10  Lectura de sensores",-99,-99,-99,-99);
      //int hume = Dht.readHumidity();      // lectura de humedad interior
      //int temp = Dht.readTemperature();   // lectura de temperatura interior
      Zig.print("#1:1:0:0:0/");           // peticion de lectura temp/hume/luz exterior a Remoto1
      break;
    case 11:
      log_v2(1,"SRV-11  Recepcion de lecturas (temp / hume / luz): ",p1,p2,p3,-99);
      // meter logica de maximas y minimas
      // meter logica de luz si control crepuscular
      break;
    case 20:
      log_v2(1,"SRV-20  Encender/Apagar releFAR: ",p1,-99,-99,-99);
      Zig.print("#1:2:" + String(p1) + ":0:0/"); 
      break;
    case 30:
      log_v2(1,"SRV-30  Encender/Apagar releCST: ",p1,-99,-99,-99);
      Zig.print("#1:3:" + String(p1) + ":0:0/");  
      break;
    case 40:
      log_v2(1,"SRV-40  Encender/Apagar releCHM: ",p1,-99,-99,-99);
      Zig.print("#1:4:" + String(p1) + ":0:0/");   
      break;
    case 50:
      log_v2(1,"SRV-50  Encender/Apagar releCRV: ",p1,-99,-99,-99);
      Zig.print("#1:5:" + String(p1) + ":0:0/");   
      break;
    case 60:
      log_v2(1,"SRV-60  Encender/Apagar todos los reles: ",p1,-99,-99,-99);
      Zig.print("#1:6:" + String(p1) + ":0:0/");   
      break;
    case 70:
      log_v2(1,"SRV-70 Mostrar display ",-99,-99,-99,-99);
      Mostrar_display();   
      break;
    default:
       log_v2(1,"SRV-xx  Servicio no definido: ", srv, p1, p2, p3);
       break;
  }
}
