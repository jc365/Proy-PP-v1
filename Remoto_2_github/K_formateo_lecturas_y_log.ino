
void formatear_msg(String in) {
  int n, id, srv, parm1, parm2, parm3;
  char *buf = in.c_str();  // copia de la direccion de msg
  n = sscanf(buf, "%d:%d:%d:%d:%d/", &id, &srv, &parm1, &parm2, &parm3);
  log_v2(3,"Formateo realizado (srv / p1 / p2 / p3): ", srv, parm1, parm2, parm3);
  if (n == 5) {
    log_v2(3,"MSG valido (5 variables parseadas)",-99,-99,-99,-99); 
    if (id == MI_ID) {
      log_v2(3,"Ejecucion local, soy el id de destino.  ",-99,-99,-99,-99); 
      ejecutar_accion(srv, parm1, parm2, parm3);  
    }
  }  
}


#ifdef DBGLVL
  void log_v2(int level, String e1, int e2, int e3, int e4, int e5)
    {
    // rutina para displayar en modo depuracion y según la constante DBGLVL en la que se fija el nivel más alto que 
    // queremos displayar: 0-nada ; 1- solo level=1; 2- level 1 y 2; 3- todos 
    // no hace nada en modo produccion
    // identa con 3 espacios cada nivel de mensaje (primer argumento de la función)
    if (level <= DBGLVL) 
      {
      String spacios = "";
      for(int n = 0 ; n < level*3 ; n++)    spacios += " ";
      String s2, s3, s4, s5 ;
      if (e2 == -99) s2 = ""; else s2 = String(e2) + " / ";
      if (e3 == -99) s3 = ""; else s3 = String(e3) + " / ";
      if (e4 == -99) s4 = ""; else s4 = String(e4) + " / ";
      if (e5 == -99) s5 = ""; else s5 = String(e5)        ;
      Serial.print(VERSION + spacios + e1 + s2 + s3 + s4 + s5 + "\n");
      }  
    }
#endif


#ifdef CHN_SRL
  void leer_Serial() 
    {
    log_v2(2,"Entrando en leerSerial()",-99,-99,-99,-99);
    String msg = "";
    char inChar;
    // posicionamiento al principio del mensaje
    while (Serial.available() && inChar != '#' ) 
      {
      inChar = Serial.read();
      log_v2(3,"Serial- Leido y desechado: "+inChar,-99,-99,-99,-99);
      } 
    // carga msg hasta final del mensaje
    while (Serial.available() && inChar != '/' ) 
      {
      inChar = Serial.read();
      msg += inChar;
      }  
    log_v2(1,"MSG leido: #"+msg,-99,-99,-99,-99);
    formatear_msg(msg);
    }
#endif 


#ifdef CHN_ZIG 
  void leer_Zig() 
    {
    log_v2(2,"Entrando en leerZig()",-99,-99,-99,-99);
    String msg = "";
    char inChar;
    // posicionamiento al principio del mensaje
    while (Zig.available() && inChar != '#' ) 
      {
      inChar = Zig.read();
      log_v2(3,"Zig- Leido y desechado: "+inChar,-99,-99,-99,-99);
      }  
    // carga msg hasta el final del mensaje
    while (Zig.available() && inChar != '/' ) 
      {
      inChar = Zig.read();
      msg += inChar;
      }  
    log_v2(1,"MSG leido: #"+msg,-99,-99,-99,-99);
    formatear_msg(msg);
    }
#endif 


#ifdef CHN_BT  
  void leer_Bt() 
    {
    log_v2(2,"Entrando en leerBt()",-99,-99,-99,-99);
    String msg = "";
    char inChar;
    // posicionamiento al principio del mensaje
    while (Bt.available() && inChar != '#' ) 
      {
      inChar = Bt.read();
      log_v2(3,"Bt- Leido y desechado: "+inChar,-99,-99,-99,-99);
      }  
    // carga msg hasta el final del mensaje
    while (Bt.available() && inChar != '/' ) 
      {
      inChar = Bt.read();
      msg += inChar;
      }  
    log_v2(1,"MSG leido: #"+msg,-99,-99,-99,-99);
    formatear_msg(msg);
    }
#endif           
 

#ifdef CHN_I2C 
  void leer_I2C() 
    {
    log_v2(2,"Entrando en leerI2C()",-99,-99,-99,-99);
    String msg = "";
    char inChar;  
    while (Wire.available() && inChar != '#' ) 
      {  // posicionamiento al principio del mensaje
      inChar = Wire.read();  log_v2(3,"I2C- Leido y desechado: "+inChar,-99,-99,-99,-99);
      }    
    while (Wire.available() && inChar != '/' ) 
      {  // carga msg hasta el final del mensaje
      inChar = Wire.read();  msg += inChar;
      }  
    log_v2(1,"I2C- MSG leido: #"+msg,-99,-99,-99,-99);
    formatear_msg(msg);
    }
    
  void escribir_I2C(String msg_out) 
    {
    Wire.beginTransmission (WIREDST);
    byte b;
    for (int n=0 ; n < msg_out.length() ; n++) 
      {
       b = msg_out[n];
       Wire.write(b);
      }
    Wire.endTransmission ();
    delay (20);
    }
#endif          

