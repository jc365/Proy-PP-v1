
void leer_luz() {
  LDR_lectura = analogRead(LDRPIN);
  if (LDR_Cambio_Luz())    LDR_Gestionar_Cambios();
}


boolean LDR_Cambio_Luz() {
  // Rutina que lee la luz, cambia el estado (DIA/NOCHE) y devuelve verdadero cuando 
  // hay cambio con respecto al ultimo estado tratado
  if (LDR_lectura > LDR_corte_operativo) LDR_estado = NOCHE ;
  else                                   LDR_estado = DIA   ;
  log_v2(2,"LDR Cambio de Luz (DIA=0 / NOCHE=-100): ",LDR_estado,-99,-99,-99); 
  if (LDR_estado != LDR_ult_tto) return true ;
  else                           return false ;
}


void LDR_Gestionar_Cambios() {
  // Rutina que enciende/apaga las luces cuando hay configurado algun sw como "crepuscular"
  // dependiendo del estado de la luz (DIA/NOCHE)
  log_v2(2,"LDR_Gestionar_Cambios",-99,-99,-99,-99); 
  if (LDR_estado == NOCHE) {
    if (LDR_sw_ent == "C") {
      log_v2(3,"Enciendo ENT",-99,-99,-99,-99); 
      // RELLENAR
    }
    if (LDR_sw_frl == "C") {
      log_v2(3,"Enciendo FRL",-99,-99,-99,-99); 
      // RELLENAR  
    }
  }
  else {
    if (LDR_sw_ent == "C") {
      log_v2(3,"Apago ENT",-99,-99,-99,-99); 
      // RELLENAR   
    }
    if (LDR_sw_frl == "C") {
      log_v2(3,"Apago FRL",-99,-99,-99,-99); 
      // RELLENAR   
    }
  }
  LDR_corte_operativo = LDR_umbral_luz + LDR_estado ;
  LDR_ult_tto = LDR_estado ;
}


void Gestionar_luz() {
  //--- medicion del sensor de luz LDR
  LDR_lectura = analogRead(LDRPIN);
  //--- fijar el estado (DIA/NOCHE) ---
  if (LDR_lectura > LDR_corte_operativo) LDR_estado = NOCHE ;
  else                                   LDR_estado = DIA   ;
  log_v2(2,"LDR Cambio de Luz (DIA=0 / NOCHE=-100): ",LDR_estado,-99,-99,-99); 
  //--- gestionar activadores  segun estado del ultimo tratamiento ---
  if (LDR_estado != LDR_ult_tto) {
    log_v2(2,"LDR_Gestionar_Cambios",-99,-99,-99,-99); 
    if (LDR_estado == NOCHE) {
      if (LDR_sw_ent == "C")      ejecutar_accion(80,0,0,0) ;
      if (LDR_sw_frl == "C")      ejecutar_accion(20,0,0,0) ;
    } else {
      if (LDR_sw_ent == "C")      ejecutar_accion(80,1,0,0) ;
      if (LDR_sw_frl == "C")      ejecutar_accion(20,1,0,0) ; 
    }
    //--- ajuste de corte para evitar falsos positivos el umbral
    LDR_corte_operativo = LDR_umbral_luz + LDR_estado ;
    LDR_ult_tto = LDR_estado ;
  }
}
