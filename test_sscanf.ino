int x;

void setup() {
  // put your setup code here, to run once:
  x = 33;
  pinMode(13, OUTPUT); 
  digitalWrite(13, LOW);
}

void loop() {

  Serial.println("hola");
  delay(5000);
}
