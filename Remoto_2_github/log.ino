
void log_v2(int level, String e1, int e2, int e3, int e4, int e5){
  // rutina para displayar en modo depuracion y según la constante DBGLVL en la que se fija el nivel más alto que 
  // queremos displayar: 0-nada ; 1- solo level=1; 2- level 1 y 2; 3- todos 
  // no hace nada en modo produccion
  // identa con 3 espacios cada nivel de mensaje (primer argumento de la función)
  if (level <= DBGLVL) {
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


