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
