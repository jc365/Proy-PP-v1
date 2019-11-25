/*
 * MACROS DEFINIDAS POR EL USUARIO
 * ===============================
 * - de formacion de macros de FECHA y HORA. Necesita la librerias: <Time.h> y <TimeLib.h> 
 * - para facilitar la depuracion. Desactivable eliminando la constante DBGLVL o igualandola a 0 (cero)
 * 
 */

//--- macros de funciones de relleno de numeros con cero(s) por delante; ademas, lo convierte a String ---
#define  RELLENA_2(a)         ( ((a) <   (10)) ? ( String("0" +    String(a)) ) : ( String(a)) ) 
#define  RELLENA_3(a)         ( ((a) <  (100)) ? ( String("0" + RELLENA_2(a)) ) : ( String(a)) ) 
#define  RELLENA_4(a)         ( ((a) < (1000)) ? ( String("0" + RELLENA_3(a)) ) : ( String(a)) ) 

//--- macros de fecha-hora en dos versiones: con el año en 4 digitos y en 2 digitos ---
#define  HORA                 RELLENA_2(hour()) + ":" + RELLENA_2(minute())
#define  FECHA_HORA_Y4        RELLENA_2(day())  + "/" + RELLENA_2(month()) + "/" + RELLENA_4(year())                + " " + HORA
#define  FECHA_HORA_Y2        RELLENA_2(day())  + "/" + RELLENA_2(month()) + "/" + RELLENA_4(year()).substring(2,4) + " " + HORA

//--- macros para facilitar trazas en tiempo de depuracion ---
#ifndef    VERSION 
  #define  VERSION             "Debug => "
#endif

#ifndef    DBGLVL 
  #define  DBGLVL              0
#endif

//--- 2 macros diferentes dependiendo del numero de parametros:
//---   - F2 requiere 2 argumentos de texto
//---   - F5 requiere 5 argumentos (un String fijo y 4 numeros)
#if DBGLVL  >  0
  #define DBGSEPARADOR         " / "
  #define DBGLOG_F2(s,t)       Serial.print(F("\n")); Serial.print(F(VERSION)); Serial.print(F(s)); Serial.print(t)
  #define DBGLOG_F5(s,a,b,c,d) Serial.print(F("\n")); Serial.print(F(VERSION)); Serial.print(F(s)); Serial.print(F(DBGSEPARADOR)); Serial.print(a); Serial.print(F(DBGSEPARADOR)); Serial.print(b); Serial.print(F(DBGSEPARADOR)); Serial.print(c); Serial.print(F(DBGSEPARADOR)); Serial.print(d)
#else
  #define DBGLOG_F2(s,t)       
  #define DBGLOG_F5(s,a,b,c,d)
#endif

