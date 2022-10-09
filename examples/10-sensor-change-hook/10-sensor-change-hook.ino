#include "KC868.h"


KC868 kc868(&Serial2); //create kc868 object

void SensorUpdate(uint8_t idx, uint8_t state)
{
  Serial.printf("Sensor %d : %d\n",idx,state); //state ,0:sensor triggered , 1:sensor not triggered
}


void setup() {

  Serial.begin(115200);

  delay(2000);

  kc868.begin(115200);//begin kc868 with baud 115200

  kc868.setSensorChangeHook(SensorUpdate);//set sensor change hook

}


void loop(){

  kc868.poll();

}