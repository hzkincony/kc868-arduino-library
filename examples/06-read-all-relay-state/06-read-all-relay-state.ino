#include "KC868.h"


KC868 kc868(&Serial2); //create kc868 object

void setup() {

  Serial.begin(115200);

  delay(2000);

  kc868.begin(115200);//begin kc868 with baud 115200

  uint32_t ret = kc868.readSwitchAll(); //read all relay state
  Serial.printf("relay state is 0x%08x.\n",ret); //32 bit value,the highest bit is the state of relay 32, and the lowest bit is the state of relay 1
}


void loop(){

  kc868.poll();

}