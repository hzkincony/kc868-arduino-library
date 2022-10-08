#include "KC868.h"


KC868 kc868(&Serial2);//create a new kc868 object, arg 1:  serial object point , arg 2: baudrate


void SwitchUpdate(uint8_t idx, uint8_t state)
{
  Serial.printf("Switch %d : %d\n",idx,state);//state ,0:switch off , 1: switch on

}

void SensorUpdate(uint8_t idx, uint8_t state)
{
  Serial.printf("Sensor %d : %d\n",idx,state); //state ,0:sensor triggered , 1:sensor not triggered
}

void setup() {

  Serial.begin(115200);

  delay(3000);

  kc868.begin(115200);//open the kc868  first

  kc868.setReadMode(0);//mode, 0 :default, Query directly; 1: Query all switch state every 2s 

  kc868.setSwitchChangeHook(SwitchUpdate);//set switch change hook

  kc868.setSensorChangeHook(SensorUpdate);//set sensor change hook

  Serial.println("Write Switch 1 on.");
  kc868.writeSwitch(1,1);

  int ret = kc868.readSwitch(1);
  Serial.printf("The state of Switch 1 is %d .\n",ret);

  Serial.println("Delay 1 second.");
  delay(1000);

  Serial.println("Write Switch 1 off.");
  kc868.writeSwitch(1,0);

  ret = kc868.readSwitch(1);
  Serial.printf("The state of Switch 1 is %d .\n",ret);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  kc868.poll();

}
