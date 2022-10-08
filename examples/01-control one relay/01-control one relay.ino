#include "KC868.h"


KC868 kc868(&Serial2); //create kc868 object

void setup() {

  Serial.begin(115200);

  delay(2000);

  kc868.begin(115200);//begin kc868 with baud 115200

  boolean ret = kc868.writeSwitch(2,1); //open relay 2 
  Serial.printf("open relay 2 ");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");

  delay(2000);

  ret = kc868.writeSwitch(2,0); //close relay 2 
  Serial.printf("close relay 2 ");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");

}


void loop(){

  kc868.poll();

}
