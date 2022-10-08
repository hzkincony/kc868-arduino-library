#include "KC868.h"


KC868 kc868(&Serial2); //create kc868 object

void setup() {

  Serial.begin(115200);

  delay(2000);

  kc868.begin(115200);//begin kc868 with baud 115200

  boolean ret = kc868.writeSwitchAll(98304); //open relay 16 and relay 17  ,hex value is 0x00018000 , decimal  value is 98304
  Serial.printf("open relay 16 and 17 ");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");

  delay(2000);

  ret = kc868.writeSwitchAll(0); //close all relay (contain relay 16 and relay 17) 
  Serial.printf("close all relay ");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");

}


void loop(){

  kc868.poll();

}