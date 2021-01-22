#include <SimpleDHT.h>

const int pinInside = 3;
const int pinOutside = 5;
const int pinRelais = 10;
const int pinLED = 12;
float absHum[] = {0,0};
SimpleDHT11 sensor;

SimpleDHT11 sensorInside(pinInside); // Model: DHT11
SimpleDHT11 sensorOutside(pinOutside); // Modell: DHT11

SimpleDHT11 sensors[] = {sensorInside, sensorOutside};

float getAbsHum(float relHum, float temp){
  // calculate absolute humidity following https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/
  const float e = 2.7182;
  float expo = 17.67*temp /(temp+243.5);
  float absHum = 13.247 * relHum *pow(e, expo)/(temp+273.15);
  return absHum;
}

void setup() {
  pinMode(pinInside, INPUT);
  pinMode(pinOutside, INPUT);
  pinMode(pinRelais, OUTPUT);
  pinMode(pinLED, OUTPUT);

}

void loop() {
  for (int i = 0; i < 2; i++){ // loop over both sensors
    sensor = sensors[i];
    // copied from SimpleDHT11 example
    byte temp = 0;
    byte relHum = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = sensor.read(&temp, &relHum, NULL)) != SimpleDHTErrSuccess) {
      return;
    }
    absHum[i]= getAbsHum(relHum, temp);
  }
  if (absHum[0] > absHum[1]){
    digitalWrite(pinRelais, HIGH);
  }
  else{
    digitalWrite(pinRelais, LOW); 
  }
  // LED flashing to indicate that the process is still running
  delay(1000);
  digitalWrite(pinLED, HIGH);
  delay(1000);
  digitalWrite(pinLED, LOW); 
}
