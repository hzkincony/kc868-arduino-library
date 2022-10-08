#include "KC868.h"


KC868 kc868(&Serial2); //create kc868 object

void setup() {

  Serial.begin(115200);

  delay(2000);

  kc868.begin(115200);//begin kc868 with baud 115200

  uint8_t ret = kc868.readSensorAll(); //read all sensor state,
  Serial.printf("sensor state is 0x%02x.\n",ret); //8 bit value,the 6st bit is the state of sensor 6, and the lowest bit is the state of sensor 1
                                                  //Which bit is 1, which bit is triggered.
}


void loop(){

  kc868.poll();

}