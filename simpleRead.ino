int aPin = 35;
int dPin = 12;
int sensorValue = 0;
float sensorAverage = 0.0;
float threshold  = 3000;
float integrator;
int numSamples = 10;
int startTime;
int count=0;
bool triggered=false;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(dPin,INPUT);
}

void loop() {
 
  /*for(int i = 0;i < numSamples; i++){
    sensorValue += analogRead(aPin);
    delay(1);
  }*/
  //sensorAverage = sensorValue / float(numSamples);
  startTime = millis();
  while (startTime + 1000 > millis()){
    sensorValue = digitalRead(dPin);
    if(sensorValue && !triggered){
      triggered = true;
    }
    else if(sensorValue && triggered){
      count++;
      triggered = false;
      
    }
  }
  //sensorValue = digitalRead(dPin);
  //Serial.println(count);
  if(count < 150000){
    Serial.println("OFF");
  }
  else if (count < 280000){
    Serial.println("Spin Cycle");
  }
  else if (count >= 280000){
    Serial.println("Crazyness");
  }
  count = 0;
  
}
