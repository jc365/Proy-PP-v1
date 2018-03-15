
void leer_luz() {
  LDR_lectura = analogRead(sensorPin);
  if (LDR_Cambio_Luz()) {
    LDR_Gestionar_Cambios();
  }
}

//----------------
// Rutina que lee la luz, cambia el estado (DIA/NOCHE) y devuelve verdadero cuando 
// hay cambio con respecto al ultimo estado tratado
//----------------
boolean LDR_Cambio_Luz() {
  if (LDR_lectura > LDR_corte_operativo) {  
    Serial.println(">> sale por noche");
    LDR_estado = NOCHE ;
  } else {  
    Serial.println(">> sale por dia");
    LDR_estado = DIA ;
  }
  if (LDR_estado != LDR_ult_tto) {
    return true ;
  } else {
    return false ;
  }
}

//----------------
// Rutina que enciende/apaga las luces cuando hay configurado algun sw como "crepuscular"
// dependiendo del estado de la luz (DIA/NOCHE)
//----------------
void LDR_Gestionar_Cambios() {
  Serial.println(">> entra a gestionar");
  if (LDR_estado == NOCHE) {
    if (LDR_sw_ent == "C") {
      // Codigo para encender. El de abajo es una simulacion 
      Serial.println(">> enciende ent");
      digitalWrite(LED_BUILTIN, HIGH);    
    }
    if (LDR_sw_frl == "C") {
      // Codigo para encender. El de abajo es una simulacion 
      Serial.println(">> enciende frl");
      digitalWrite(LED_BUILTIN, HIGH);    
    }
  }
  else {
    if (LDR_sw_ent == "C") {
      // Codigo para apagar. El de abajo es una simulacion 
      Serial.println(">> apaga ent");
      digitalWrite(LED_BUILTIN, LOW);    
    }
    if (LDR_sw_frl == "C") {
      // Codigo para apagar. El de abajo es una simulacion 
      Serial.println(">> apaga frl");
      digitalWrite(LED_BUILTIN, LOW);    
    }
  }
  LDR_corte_operativo = LDR_umbral_luz + LDR_estado ;
  LDR_ult_tto = LDR_estado ;
  return ;
}
