void setup(){
  Serial.begin(11520);
}
void loop(){
  int rpm =  random(10000);
  int speed =  random(100);
  int motorTemp =  random(80);
  int accuTemp =  random(80);
  int hv =  random(300);
  int lv =  random(12);

    Serial.println(String(rpm) + "," + String(lv) + "," + String(hv) + "," +String(motorTemp) + "," + String(accuTemp) + "," + String(speed));
  }