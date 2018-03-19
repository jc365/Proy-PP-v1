void Gestionar_luz() {
  //--- medicion del sensor de luz LDR
  LDR_lectura = analogRead(LDRPIN);
  //--- fijar el estado (DIA/NOCHE) ---
  if (LDR_lectura > LDR_corte_op) LDR_estado = NOCHE ;
  else                            LDR_estado = DIA   ;
  log_v2(3,"LDR- Fijado estado de la Luz (DIA=0 / NOCHE=-100): ",LDR_estado,-99,-99,-99); 
  //--- gestionar activadores  segun estado del ultimo tratamiento ---
  if (LDR_estado != LDR_ult_tto) {
    log_v2(3,"LDR- Ha cambiado el estado",-99,-99,-99,-99); 
    //--- ajuste de corte para evitar falsos positivos el umbral
    LDR_corte_op = LDR_umbral_luz + LDR_estado ;
    LDR_ult_tto = LDR_estado ;
  }
}


