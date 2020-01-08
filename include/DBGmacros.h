/*
 * MACROS Y RUTINAS DEFINIDAS POR EL USUARIO
 * ==========================================
 * a) de FECHA y HORA. Necesita la libreria: <Time.h> y <TimeLib.h>
 * b) para facilitar la depuracion. Desactivable eliminando la constante DBGLVL o igualandola a 0 (cero)
 * c) rutinas comunes para cualquier estacion que la necesite
 */
 
#include <Time.h>
#include <TimeLib.h>

//--- macros de funciones de relleno de numeros con cero(s) por delante; ademas, lo convierte a String ---
#define  RELLENA_2(a)         ( ((a) <   (10)) ? ( String("0" +    String(a)) ) : ( String(a)) )
#define  RELLENA_3(a)         ( ((a) <  (100)) ? ( String("0" + RELLENA_2(a)) ) : ( String(a)) )
#define  RELLENA_4(a)         ( ((a) < (1000)) ? ( String("0" + RELLENA_3(a)) ) : ( String(a)) )

//--- macros de fecha-hora en dos versiones: con el año en 4 digitos y en 2 digitos ---
#define  HORA                 RELLENA_2(hour()) + ":" + RELLENA_2(minute())
#define  FECHA_HORA_Y4        RELLENA_2( day()) + "/" + RELLENA_2( month()) + "/" + RELLENA_4(year())                + " " + HORA
#define  FECHA_HORA_Y2        RELLENA_2( day()) + "/" + RELLENA_2( month()) + "/" + RELLENA_4(year()).substring(2,4) + " " + HORA

//--- macros para facilitar trazas en tiempo de depuracion ---
#ifndef    VERSION
  #define  VERSION             "Debug => "
#endif

#ifndef    DBGLVL
  #define  DBGLVL              0
#endif

//--- 2 macros diferentes dependiendo del numero de parametros:
//---   _F2 requiere 2 argumentos de texto: el primero es un literal; el segundo es una variable String
//---   _F5 requiere 5 argumentos (un literal y 4 numeros)
#if DBGLVL  >  0
  #define DBGSEPARADOR         " / "
  #define DBGLOG_F2(s,t)       Serial.print(F("\n")); Serial.print(F(VERSION)); Serial.print(F(s)); Serial.print(t)
  #define DBGLOG_F5(s,a,b,c,d) Serial.print(F("\n")); Serial.print(F(VERSION)); Serial.print(F(s)); Serial.print(F(DBGSEPARADOR)); Serial.print(a); Serial.print(F(DBGSEPARADOR)); Serial.print(b); Serial.print(F(DBGSEPARADOR)); Serial.print(c); Serial.print(F(DBGSEPARADOR)); Serial.print(d)
#else
  #define DBGLOG_F2(s,t)
  #define DBGLOG_F5(s,a,b,c,d)
#endif

#ifndef ARDPRINTF
#define ARDPRINTF
#define ARDBUFFER 16
#include <stdarg.h>
#include <Arduino.h>

int ardprintf(char *str, ...)
{
  int i, contador=0, j=0;
  char temp[ARDBUFFER+1];
  for(i=0; str[i]!='\0';i++)  if(str[i]=='%')  contador++;

  va_list argv;
  va_start(argv, contador);
  for(i=0,j=0; str[i]!='\0';i++)
  {
    if(str[i]=='%')
    {
      temp[j] = '\0';
      Serial.print(temp);
      j=0;
      temp[0] = '\0';

      switch(str[++i])
      {
        case 'd': Serial.print(va_arg(argv, int));
                  break;
        case 'l': Serial.print(va_arg(argv, long));
                  break;
        case 'f': Serial.print(va_arg(argv, double));
                  break;
        case 'c': Serial.print((char)va_arg(argv, int));
                  break;
        case 's': Serial.print(va_arg(argv, char *));
                  break;
        default:  ;
      };
    }
    else
    {
      temp[j] = str[i];
      j = (j+1)%ARDBUFFER;
      if(j==0)
      {
        temp[ARDBUFFER] = '\0';
        Serial.print(temp);
        temp[0]='\0';
      }
    }
  };
  Serial.println();
  return contador + 1;
}
#undef ARDBUFFER
#endif
