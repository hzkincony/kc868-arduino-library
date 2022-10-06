#ifndef _KC868_
#define _KC868_

#include "Arduino.h"
#include "HardwareSerial.h"
#include "driver/uart.h"


#define ON 1
#define OFF 0
#define PKT_MAX_LEN 128

#define OK_CMD "OK"
#define ERROR_CMD "ERROR"
#define SET_RELAY_CMD "RELAY-SET-255"
#define SET_RELAY_ALL_CMD "RELAY-SET_ALL-255"
#define READ_RELAY_CMD "RELAY-READ-255"
#define KEY_RELAY_CMD "RELAY-KEY-255"
#define RELAY_CMD "RELAY-"
#define RELAY_STATE_CMD "RELAY-STATE-255\n"
#define RELAY_STATE_ALL_ON "RELAY-AON-255,1,1"
#define RELAY_STATE_ALL_OFF "RELAY-AOF-255,1,1"
#define RELAY_GET_INPUT "RELAY-GET_INPUT-255"


#define TIME_PERIOD     2000ul   //ms

typedef enum
{
  dev_serial,
  dev_tcp,
  dev_udp
} _dev_type;

typedef struct
{
  char txbuf[PKT_MAX_LEN];
  uint16_t txlen;
  char rxbuf[PKT_MAX_LEN];
  uint16_t rxlen;
} _data_pkt;

typedef void (*hook)(uint8_t, uint8_t);

typedef struct
{
  char *head;
  char *p1;
  char *p2;
  char *p3;
  char *p4;
  char *state;
} _cmd_pkt;

class KC868
{

  _dev_type _type;
  HardwareSerial *_serial;
  int _baud;
  uint32_t _SwitchCache;//save switch state ,32 bit
  uint32_t _SwitchCache_last=0x00;//save last switch state ,32 bit
  uint8_t _SensorCache=0XFF;//save sensor input state 

  uint8_t _mode;
  hw_timer_t*  tim= NULL;
  hook SwitchChangeHook=NULL,SensorChangeHook=NULL;

  uint16_t bufferPos = 0;
  uint16_t readPos = 0;
  char inputBuffer[PKT_MAX_LEN];

  uint8_t _totalRelayCount=32;

  void write(char *data, uint16_t len);
  void read(char *data, uint16_t *len, uint16_t timeout = 100);
  boolean checkRet(char *src, uint16_t timeout = 100);
  void handleData(char *data, uint16_t len);
  uint8_t timeFlag;
  unsigned long nowTime;
  unsigned long lastTime;

  _data_pkt pkt;

public:
  KC868(HardwareSerial *hs, unsigned long baud); //serial
  // KC868(WiFiClient client);         // wificlient
  
  boolean open();
  boolean close();
  void poll();
  boolean readSwitch(int idx);
  boolean readSwitchCache(int idx);
  uint32_t readSwitchAll();
  boolean writeSwitch(int idx, int value);
  boolean writeSwitchAll(uint32_t value);
  boolean writeSwitchAllON();
  boolean writeSwitchAllOFF();
  boolean readSensor(int idx);
  uint8_t readSensorAll();
  void setSwitchChangeHook(hook func);
  void setSensorChangeHook(hook func);
  void setReadMode(uint8_t mode);
};

#endif
