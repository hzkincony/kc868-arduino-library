#include "KC868.h"


KC868 kc868(&Serial2); //create kc868 object

void setup() {

  Serial.begin(115200);

  delay(2000);

  kc868.begin(115200);//begin kc868 with baud 115200

  uint8_t ret = kc868.readSwitch(15); //read relay 15
  Serial.printf("relay 15 state is %d.\n",ret); //ret=1 relay 15 on, ret=0 relay 15 off

}


void loop(){

  kc868.poll();

}