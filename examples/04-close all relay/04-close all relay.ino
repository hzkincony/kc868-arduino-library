#include "KC868.h"


KC868 kc868(&Serial2); //create kc868 object

void setup() {

  Serial.begin(115200);

  delay(2000);

  kc868.begin(115200);//begin kc868 with baud 115200

  boolean ret = kc868.writeSwitchAllOFF(); //close all relay
  Serial.printf("close all relay ");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");

}


void loop(){

  kc868.poll();

}