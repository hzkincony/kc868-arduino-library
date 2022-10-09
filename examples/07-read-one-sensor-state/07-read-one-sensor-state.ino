#include "KC868.h"


KC868 kc868(&Serial2); //create kc868 object

void setup() {

  Serial.begin(115200);

  delay(2000);

  kc868.begin(115200);//begin kc868 with baud 115200

  uint8_t ret = kc868.readSensor(5); //read sensor 5 state,
  Serial.printf("sensor 5 state is %d.\n",ret); //ret=1,the sensor is tirggered, ret=0,the sensor is not tirggered

}


void loop(){

  kc868.poll();

}