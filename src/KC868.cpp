#include "KC868.h"

KC868::KC868(HardwareSerial *hs, unsigned long baud)
{
  _type = dev_serial;
  _serial = hs;
  _baud = baud;
}

boolean KC868::open()
{
  if (_type == dev_serial)
  {
    _serial->begin(_baud);

  }

  return true;
}

boolean KC868::close()
{
  if (_type == dev_serial)
  {
    _serial->end();
  }
  return true;
}

boolean KC868::readSwitch(int idx)
{
  boolean ret;
  char tmp[PKT_MAX_LEN];


  if(_mode == 1)
  {
    return readSwitchCache(idx);
  }

  if(idx>_totalRelayCount)
    return false;

  // example  RELAY-READ-255,1
  sprintf(pkt.txbuf, "%s,%d\n", READ_RELAY_CMD, idx);
  write(pkt.txbuf, strlen(pkt.txbuf));

  // example  RELAY-READ-255,1,1,OK\0
  sprintf(tmp, "%s,%d,1,OK", READ_RELAY_CMD, idx); //check 'OK' ,otherwise return false
  ret = checkRet(tmp);
  Serial.printf("---%s---",pkt.rxbuf);
  Serial.printf("---%d---",ret);
  if (ret)
    return true;
  else
    return false;
}

boolean KC868::readSwitchCache(int idx)
{
  boolean ret;
  char tmp[PKT_MAX_LEN];

  if(idx>_totalRelayCount)
    return false;

  if(_SwitchCache&(0x00000001<<(idx-1)))
    return true;
  else
    return false;
}

uint32_t KC868::readSwitchAll()
{
  boolean ret;
  char *p1,*p2;
  _cmd_pkt cmd;
  int dotCount=0;

  if(_mode == 1)
  {
    return _SwitchCache;
  }

  // example  RELAY-STATE-255\n
  sprintf(pkt.txbuf, "%s", RELAY_STATE_CMD);
  write(pkt.txbuf, strlen(pkt.txbuf));

  // example  RELAY-STATE-255,0,0,0,1,OK
  ret = checkRet("RELAY-STATE-255");
  if (ret)
  {
    // find return string  from pkt.rxbuf 
      p1 = strstr(pkt.rxbuf, RELAY_CMD);
  if (p1 != NULL)
  {

    p2 = strstr(p1, OK_CMD);
    if ((p2 != NULL) && (p2 - p1 >= 14))
    {
      cmd.head = p1;
      cmd.state = p2;
      
      for (int n = 0; n < (p2 - p1); n++)
      {
        if (p1[n] == ',')
        {
          p1[n] = '\0';
          dotCount++;
          if (dotCount == 1)
            cmd.p1 = &p1[n + 1];
          else if (dotCount == 2)
            cmd.p2 = &p1[n + 1];
          else if (dotCount == 3)
            cmd.p3 = &p1[n + 1];
          else if (dotCount == 4)
            cmd.p4 = &p1[n + 1];
        }
      }

      if(strstr(cmd.head,"RELAY-STATE-255")!=NULL)
      {
        uint8_t n1 = 0, n2 = 0,n3=0,n4=0;
        if(_totalRelayCount == 8)
        {
          n1 = atoi(cmd.p1);
 
        }
        else if(_totalRelayCount == 16)
        {
          n2 = atoi(cmd.p1);
          n1 = atoi(cmd.p2);

        }
        else if(_totalRelayCount == 32)
        {
          n4 = atoi(cmd.p1);
          n3 = atoi(cmd.p2);
          n2 = atoi(cmd.p3);
          n1 = atoi(cmd.p4);
        }

        _SwitchCache=n4;
        _SwitchCache<<=8;
        _SwitchCache|=n3;
        _SwitchCache<<=8;
        _SwitchCache|=n2;
        _SwitchCache<<=8;
        _SwitchCache|=n1;

        return _SwitchCache;
      }
    }
  }

    
  }

  return 0;

}


boolean KC868::writeSwitch(int idx, int value)
{
  boolean ret;
  uint8_t data;
  char tmp[PKT_MAX_LEN];

  if(idx>_totalRelayCount)
      return false;

  // example  RELAY-SET-255,1,1
  sprintf(pkt.txbuf, "%s,%d,%d\n", SET_RELAY_CMD, idx, value);
  write(pkt.txbuf, strlen(pkt.txbuf));
  delay(80);//delay at least 80ms, the control board needs some time to process


  // example  RELAY-SET-255,1,1,OK
  // sprintf(tmp,"%s,%d,%d,OK", SET_RELAY_CMD, idx, value); //check 'OK'，otherwise return false
  // ret = checkRet(tmp);
  // if (ret)
  // {
  //   //update cache
  //   if(value==1)
  //   {
  //     _SwitchCache|=(0x00000001<<(idx-1));
  //   }
  //   else
  //   {
  //     _SwitchCache&=(~(0x00000001<<(idx-1)));
  //   }

  //   return true;
  // }
    
  // else
  //   return false;
  return true;
}

boolean KC868::writeSwitchAll(uint32_t value)
{
  boolean ret;
  uint8_t data;
  char tmp[PKT_MAX_LEN];
  uint8_t r1=0,r2=0,r3=0,r4=0;

  if(_totalRelayCount == 8)
  {
    r1 = value & 0xFF;
    sprintf(pkt.txbuf, "%s,%d\n", SET_RELAY_ALL_CMD, r1);

  }
  
  if(_totalRelayCount==16)
  {
    r1 = value & 0xFF;
    r2 = (value>>8) & 0xFF;
    sprintf(pkt.txbuf, "%s,%d,%d\n", SET_RELAY_ALL_CMD, r2,r1);
  }
    
  if(_totalRelayCount==32)
  {
    r1 = value & 0xFF;
    r2 = (value>>8) & 0xFF;
    r3 = (value>>16) & 0xFF;
    r4 = (value>>24) & 0xFF;
  sprintf(pkt.txbuf, "%s,%d,%d,%d,%d\n", SET_RELAY_ALL_CMD,r4,r3,r2,r1);
  }
  // example  RELAY-SET-255,1,1
  write(pkt.txbuf, strlen(pkt.txbuf));
  //delay(80);
  pkt.txbuf[strlen(pkt.txbuf)-1]=0;
  sprintf(tmp,"%s,OK", pkt.txbuf); //check 'OK'，otherwise return false
  ret = checkRet(tmp);
  if (ret)
  {
    return true;
  }
    
  else
    return false;

}

boolean KC868::writeSwitchAllON()
{
  boolean ret;
  uint8_t data;
  char tmp[PKT_MAX_LEN];


  sprintf(pkt.txbuf, "%s\n", RELAY_STATE_ALL_ON);
  
  write(pkt.txbuf, strlen(pkt.txbuf));
  //delay(80);
  sprintf(tmp,"%s,OK", RELAY_STATE_ALL_ON); //check 'OK'，otherwise return false
  ret = checkRet(tmp);
  if (ret)
  {
    return true;
  }
    
  else
    return false;

}

boolean KC868::writeSwitchAllOFF()
{
  boolean ret;
  uint8_t data;
  char tmp[PKT_MAX_LEN];


  sprintf(pkt.txbuf, "%s\n", RELAY_STATE_ALL_OFF);
  write(pkt.txbuf, strlen(pkt.txbuf));
  //delay(80);
  sprintf(tmp,"%s,OK", RELAY_STATE_ALL_OFF); //check 'OK'，otherwise return false
  ret = checkRet(tmp);
  if (ret)
  {
    return true;
  }
    
  else
    return false;

}

//return sensor state , 0 happened , 1 not happened
boolean KC868::readSensor(int idx)
{
  boolean ret;
  uint8_t value = 0;
  uint8_t data;
  char *p1,*p2;
  _cmd_pkt cmd;

  sprintf(pkt.txbuf, "%s\n", RELAY_GET_INPUT);
  write(pkt.txbuf, strlen(pkt.txbuf));
  //delay(80);
  ret = checkRet(RELAY_GET_INPUT);
  if (ret)
  {
    p1 = strstr(pkt.rxbuf, RELAY_CMD);
    if (p1 != NULL)
    {
      //RELAY-GET_INPUT-255,255,OK
      p2 = strstr(p1, OK_CMD);

      if ((p2 != NULL) && (p2 - p1 >= 22))
      {
        cmd.head = p1;
        cmd.state = p2;
        cmd.p1 = &p1[20];
        for(int n=20;n<24;n++)
        {
          if (p1[n] == ',')
          {
            p1[n] = '\0';
          }
        }

        value = atoi(cmd.p1);
        value = (value>>(idx-1))&0x01;
        return value;
      }
    }
  }
    
  return 1;

}

//return 6 sensor state in 1 byte,  D5-D0 
uint8_t KC868::readSensorAll()
{
  boolean ret;
  uint8_t value = 0;
  uint8_t data;
  char *p1,*p2;
  _cmd_pkt cmd;

  sprintf(pkt.txbuf, "%s\n", RELAY_GET_INPUT);
  write(pkt.txbuf, strlen(pkt.txbuf));
  //delay(80);
  ret = checkRet(RELAY_GET_INPUT);
  if (ret)
  {
    p1 = strstr(pkt.rxbuf, RELAY_CMD);
    if (p1 != NULL)
    {
      //RELAY-GET_INPUT-255,255,OK
      p2 = strstr(p1, OK_CMD);
      if ((p2 != NULL) && (p2 - p1 >= 22))
      {
        cmd.head = p1;
        cmd.state = p2;
        cmd.p1 = &p1[20];
        for(int n=20;n<24;n++)
        {
          if (p1[n] == ',')
          {
            p1[n] = '\0';
          }
        }

        value = atoi(cmd.p1);
        return value;
      }
    }
  }
    
  return 0xFF;

}

void KC868::write(char *data, uint16_t len)
{
  if (_type == dev_serial)
  {
    memset(pkt.rxbuf, 0, PKT_MAX_LEN);
    _serial->write(data, len);
  }
}

void KC868::read(char *data, uint16_t *len, uint16_t timeout)
{
  if (_type == dev_serial)
  {
    _serial->readBytesUntil('\n', data, timeout);
    *len = strlen(data);
  }
}

boolean KC868::checkRet(char *src, uint16_t timeout)
{
  //char buf[PKT_MAX_LEN];
  uint16_t len = 0;
  if (_type == dev_serial)
  {
    _serial->readBytesUntil('\n', pkt.rxbuf, timeout);
    len = strlen(pkt.rxbuf);
    if (len < strlen(src))
      return false;
    if (strstr(pkt.rxbuf, src) != NULL)
      return true;
  }

  return false;
}

void KC868::poll()
{
  if(_mode == 1)
  {
    nowTime = millis();
    if(nowTime>lastTime?(nowTime-lastTime >= TIME_PERIOD):(nowTime >= lastTime+TIME_PERIOD))
    {
      lastTime = millis();
      timeFlag = 1;
    }

    if(timeFlag)
    {
      timeFlag = 0;
      write(RELAY_STATE_CMD, strlen(RELAY_STATE_CMD));
    }
    
  }
  

  while (_serial->available())
  {

    uint8_t received = _serial->read();
      if((received=='\0')||(received=='\n'))
        break;
      inputBuffer[bufferPos++] = received;
      if (bufferPos >= PKT_MAX_LEN)
      {
        bufferPos = 0;
      }
    
  }
  if(bufferPos>0)
  {
    //handle one line
    handleData(inputBuffer, bufferPos);

    memset(inputBuffer, 0, PKT_MAX_LEN);
    bufferPos = 0;
  }
  
  

}

void KC868::setSwitchChangeHook(hook func)
{
  SwitchChangeHook = func;
}

void KC868::setSensorChangeHook(hook func)
{
  SensorChangeHook = func;
}

//example   RELAY-KEY-255,x,y,OK    
//          RELAY-ALARM-x,OK        alarm
//          RELAY-STATE-255,D3,D2,D1,D0,OK   set all relay return
void KC868::handleData(char *data, uint16_t len)
{
  int dotCount = 0;
  _cmd_pkt cmd;
  char *p1, *p2;

  p1 = strstr(data, RELAY_CMD);
  if (p1 != NULL)
  {
    lastTime = millis();//update last time

    p2 = strstr(p1, OK_CMD);
    if ((p2 != NULL) && (p2 - p1 >= 14))
    {
      cmd.head = p1;
      cmd.state = p2;
      //analysis  string
      for (int n = 0; n < (p2 - p1); n++)
      {
        if (p1[n] == ',')
        {
          p1[n] = '\0';
          dotCount++;
          if (dotCount == 1)
            cmd.p1 = &p1[n + 1];
          else if (dotCount == 2)
            cmd.p2 = &p1[n + 1];
          else if (dotCount == 3)
            cmd.p3 = &p1[n + 1];
          else if (dotCount == 4)
            cmd.p4 = &p1[n + 1];
        }
      }

      if(strstr(cmd.head,"RELAY-STATE-255")!=NULL)
      {
        uint8_t n1 = 0, n2 = 0,n3=0,n4=0;
        if(_totalRelayCount == 8)
        {
          n1 = atoi(cmd.p1);
 
        }
        else if(_totalRelayCount == 16)
        {
          n2 = atoi(cmd.p1);
          n1 = atoi(cmd.p2);

        }
        else if(_totalRelayCount == 32)
        {
          n4 = atoi(cmd.p1);
          n3 = atoi(cmd.p2);
          n2 = atoi(cmd.p3);
          n1 = atoi(cmd.p4);
        }

        _SwitchCache=n4;
        _SwitchCache<<=8;
        _SwitchCache|=n3;
        _SwitchCache<<=8;
        _SwitchCache|=n2;
        _SwitchCache<<=8;
        _SwitchCache|=n1;

        if(_SwitchCache_last!=_SwitchCache)
        {


          for(uint8_t n=0;n<32;n++)
          {
            if(((_SwitchCache>>n)&0x01)!=((_SwitchCache_last>>n)&0x01))
            {
              if(SwitchChangeHook!=NULL)
                SwitchChangeHook(n+1, ((_SwitchCache>>n)&0x01));//call switch change hook too

            }

          }

          _SwitchCache_last = _SwitchCache;



        }

        //Serial.printf("relay : %08x",_SwitchCache);
        
      }
      else if(strstr(cmd.head,"RELAY-SET-255")!=NULL)
      {
        uint8_t idx = 0, value = 0;

        idx = atoi(cmd.p1);
        value = atoi(cmd.p2);
        if ((idx <= 32) && (value <= 1))
        {
          if(value==1)
            _SwitchCache|=(0x00000001<<(idx-1));
          else
            _SwitchCache&= (~(0x00000001<<(idx-1)));

          if(SwitchChangeHook!=NULL)
            SwitchChangeHook(idx, value);//switch change hook
        }
          
      }
      //RELAY-ALARM-1,OK
      else if(strstr(cmd.head,"RELAY-ALARM-")!=NULL)
      {
        uint8_t idx = 0;
        p1 = strstr(data, "ALARM-");
        if (p1 != NULL)
        {

          cmd.p1 = p1+6;
          idx = atoi(cmd.p1);

          if(SensorChangeHook!=NULL)
            SensorChangeHook(idx,0);//alarm

        }

      }
      else if(strstr(cmd.head,"RELAY-DIARM-")!=NULL)
      {
        uint8_t idx = 0;

        p1 = strstr(data, "DIARM-");
        if (p1 != NULL)
        {
          cmd.p1 = p1+6;

          idx = atoi(cmd.p1);

          if(SensorChangeHook!=NULL)
            SensorChangeHook(idx,1);//Cancel alarm


        }

      }
      

    }
  }
}

//0 default mode     1 Query every 2 seconds
void KC868::setReadMode(uint8_t mode)
{

  _mode = mode;
  if(_mode == 1)
  {
    timeFlag = 1;
  }
  

}


