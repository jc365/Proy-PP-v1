#define VERBOSE

#ifdef VERBOSE
#define VERBOSE_PRINT(str) Serial.println(str)
#define VERBOSE_PRINT2(str) Serial.println(str); Serial.println("ghgh")
#else
#define VERBOSE_PRINT(str)
#endif

#define DBGLVL   1

#if DBGLVL > 0
  #define DBGprint(s)       Serial.print(F("\n")); Serial.print(F(s))
  #define DBGprint1V(a)     Serial.print(F(" >")); Serial.print(a)
  #define DBGprint2V(a,b)   Serial.print(F(" >")); Serial.print(a); Serial.print(F(" >")); Serial.print(b); 
  #define DBGprint3V(a,b,c) Serial.print(F(" >")); Serial.print(a); Serial.print(F(" >")); Serial.print(b); Serial.print(F(">")); Serial.print(c)
#else
  #define DBGprint(s)       
  #define DBGprint1V(a)     
  #define DBGprint2V(a,b)   
  #define DBGprint3V(a,b,c) 
#endif

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  delay(2000);
  DBGprint("plastillo");
  DBGprint("78");
  DBGprint3V(89,22,33);
  DBGprint2V(5,6);
  DBGprint1V(77);

  // put your main code here, to run repeatedly:

}
