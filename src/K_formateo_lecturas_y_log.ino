
//---   RUTINAS COMUNES PARA TODAS LAS ESTACIONES DEL PROYECTO (PRINCIPAL Y REMOTOS)
//---   unas son fijas para todas, otras dependen de si la estacion dispone o no 
//---   de la linea serie apropiada, todo segun el valor de ciertas constantes

//---   Parsea los mensajes recibidos desde cualquier linea serie
void formatear_msg(String in) {
  int n, id, srv, parm1, parm2, parm3;
  char *buf = in.c_str();  // copia de la direccion de msg
  n = sscanf(buf, "%d:%d:%d:%d:%d/", &id, &srv, &parm1, &parm2, &parm3);
  DBGLOG_F5("Formateo realizado (srv / p1 / p2 / p3): ", srv, parm1, parm2, parm3);
  if (n == 5) {
    DBGLOG_F2("MSG valido (5 variables parseadas)",""); 
    if (id == MI_ID) {
      DBGLOG_F2("Ejecucion local, soy el id de destino.  ","\n"); 
      ejecutar_accion(srv, parm1, parm2, parm3);  
    }
  }  
}


//--- Rutina de lectura SERIAL
#if CHN_SRL
void leer_Serial() 
  {
  DBGLOG_F2("Entrando en leerSerial()","");
  String msg = "";
  char inChar;
  // posicionamiento al principio del mensaje
  while (Serial.available() && inChar != '#' ) 
    {
    inChar = Serial.read();
    DBGLOG_F2("Serial- Leido y desechado: ",inChar);
    } 
  // carga msg hasta final del mensaje
  while (Serial.available() && inChar != '/' ) 
    {
    inChar = Serial.read();
    msg += inChar;
    }  
  DBGLOG_F2("MSG leido: #",msg);
  formatear_msg(msg);
  }
#endif 


//--- Rutina de lectura ZigBee
#if CHN_ZIG 
  void leer_Zig() 
    {
    DBGLOG_F2("Entrando en leerZig()","");
    String msg = "";
    char inChar;
    // posicionamiento al principio del mensaje
    while (Zig.available() && inChar != '#' ) 
      {
      inChar = Zig.read();
      DBGLOG_F2("Zig- Leido y desechado: ",inChar);
      }  
    // carga msg hasta el final del mensaje
    while (Zig.available() && inChar != '/' ) 
      {
      inChar = Zig.read();
      msg += inChar;
      }  
    DBGLOG_F2("MSG leido: #",msg);
    formatear_msg(msg);
    }
#endif 


//--- Rutina de lectura Bluetooth
#if CHN_BT  
  void leer_Bt() 
    {
    DBGLOG_F2("Entrando en leerBt()","");
    String msg = "";
    char inChar;
    // posicionamiento al principio del mensaje
    while (Bt.available() && inChar != '#' ) 
      {
      inChar = Bt.read();
      DBGLOG_F2("Bt- Leido y desechado: ",inChar);
      }  
    // carga msg hasta el final del mensaje
    while (Bt.available() && inChar != '/' ) 
      {
      inChar = Bt.read();
      msg += inChar;
      }  
    DBGLOG_F2("MSG leido: #",msg);
    formatear_msg(msg);
    }
#endif           
 

//--- Rutina de lectura y escritura I2C
#if CHN_I2C 
  void leer_I2C() 
    {
    DBGLOG_F2("Entrando en leerI2C()","");
    String msg = "";
    char inChar;  
    while (Wire.available() && inChar != '#' ) 
      {  // posicionamiento al principio del mensaje
      inChar = Wire.read();  DBGLOG_F2("I2C- Leido y desechado: ",inChar);
      }    
    while (Wire.available() && inChar != '/' ) 
      {  // carga msg hasta el final del mensaje
      inChar = Wire.read();  msg += inChar;
      }  
    DBGLOG_F2("I2C- MSG leido: #",msg);
    formatear_msg(msg);
    }
    
  void escribir_I2C(String msg_out) 
    {
    DBGLOG_F2("Entrando en escribirI2C() para enviar: ",msg_out);
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

