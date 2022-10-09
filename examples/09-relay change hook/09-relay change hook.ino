#include "KC868.h"


KC868 kc868(&Serial2); //create kc868 object

void SwitchUpdate(uint8_t idx, uint8_t state)
{
  Serial.printf("Switch %d : %d\n",idx,state);//state ,0:switch off , 1: switch on

}

void setup() {

  Serial.begin(115200);

  delay(2000);

  kc868.begin(115200);//begin kc868 with baud 115200

  kc868.setSwitchChangeHook(SwitchUpdate);//set switch change hook

}


void loop(){

  kc868.poll();

}