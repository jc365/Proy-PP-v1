
void formatear_msg(String in) {
  int n, id, srv, parm1, parm2, parm3;
  char *buf = in.c_str();  // copia de la direccion de msg
  n = sscanf(buf, "%d:%d:%d:%d:%d/", &id, &srv, &parm1, &parm2, &parm3);
  DBGLOG_F5("Formateo realizado (srv / p1 / p2 / p3): ", srv, parm1, parm2, parm3);
  if (n == 5) {
    DBGLOG_F5("MSG valido (5 variables parseadas)",-99,-99,-99,-99); 
    if (id == MI_ID) {
      DBGLOG_F5("Ejecucion local, soy el id de destino.  ",-99,-99,-99,-99); 
      ejecutar_accion(srv, parm1, parm2, parm3);  
    }
  }  
}


#if CHN_SRL
void leer_Serial() 
  {
  DBGLOG_F5("Entrando en leerSerial()",-99,-99,-99,-99);
  String msg = "";
  char inChar;
  // posicionamiento al principio del mensaje
  while (Serial.available() && inChar != '#' ) 
    {
    inChar = Serial.read();
    DBGLOG_F2("Serial- Leido y desechado: ",inChar);
    DBGLOG_F5("Serial- Leido y desechado: ",-99,-99,-99,-99);
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


#if CHN_ZIG 
  void leer_Zig() 
    {
    DBGLOG_F5("Entrando en leerZig()",-99,-99,-99,-99);
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


#if CHN_BT  
  void leer_Bt() 
    {
    DBGLOG_F5("Entrando en leerBt()",-99,-99,-99,-99);
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
 

#if CHN_I2C 
  void leer_I2C() 
    {
    DBGLOG_F5("Entrando en leerI2C()",-99,-99,-99,-99);
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

