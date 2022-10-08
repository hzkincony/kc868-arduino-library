# kc868-arduino-library  API Reference
[English](https://github.com/hzkincony/kc868-arduino-library/blob/main/API_Reference_EN.md)|[中文](https://github.com/hzkincony/kc868-arduino-library/blob/main/API_Reference_CN.md)

---

This library for KC868-H32B Pro smart controller based on ESP32 chip, you should include "KC868.h" in your code as follows:

```C++
#include "KC868.h"
```

---

## function list

- **KC868(HardwareSerial \*hs)**

  - KC868 needs to create a global object before it can be used. After creating this object, you can use the following methods to implement functions. You can use `KC868()` to create this object. The code is as follows:
  
  ```c++
  KC868 kc868(&Serial2);
  ```

  - You should use ESP32 hardware serial port with kc868 function for initialization.

- **boolean begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms, uint8_t rxfifo_full_thrhd)**

  - begin function is to open the serial port. Its parameter as same as arduino official serial port. The first baud rate is a parameter that must be passed in. Everything else can be omitted. You just need to simply use the following code to complete the initialization.
  
  ```c++
  kc868.begin(115200);
  ```

  - If you want to use any IO ports as serial ports, you can use the following code:

  ```c++
  kc868.begin(115200,SERIAL_8N1,22,23);
  ```

  - GPIO22 is RXD pin, GPIO23 is TXD pin.

- **boolean close()**

  - Close the serial port of kc868
  
- **void poll()**

  - To receive the reported data from kc868 and send query messages regularly, this function must be placed in the `loop()` function.
  
- **boolean readSwitch(int idx)**

  - Read the relay status of the specified channel number, the parameter range is 1-32, corresponding to 32 relays.
  - The return value is of type `boolean`, `true` is relay ON, and `flase` is relay OFF.
  - Code example:
  
  ```c++
  int ret = kc868.readSwitch(1);
  Serial.printf("The value of relay 1 is %d .\n",ret);
  ```

- **boolean readSwitchCache(int idx)**
  
  - Read the relay status of the specified channel number, the parameter range is 1-32, corresponding to 32 relays.
  - The return value is of type `boolean`, `true` is relay ON, and `flase` is relay OFF.
  - what different `readSwitch`, this one returns the value of the relay held in memory directly, rather than reading it in real time.
  
- **uint32_t readSwitchAll()**

  - Read the status of 32 channel relays.
  - The return value is of type `uint32_t`, with a total of 32 bits, the highest bit is the state of relay 32, and the lowest bit is the state of relay 1. For example, the return value is 0x00000001, which means that relay1 is ON, and the return value is 0x00000003, which means that the relay1 and relay2 are both ON.
  - Code example:

  
  ```c++
  uint32_t ret = kc868.readSwitchAll();
  Serial.printf("The value of all relay is %d .\n",ret);
  ```

- **boolean writeSwitch(int idx, int value)**

  - Turn ON/OFF specified channel relay. The range of parameter 'idx' is 1-32, corresponding to 32 relays; the value of parameter value is 0 and 1, 0 is OFF, 1 is ON
  - The return value is of type `boolean`, `true` means the operation succeeded, `false` means the operation failed.
  - Code example:

  
  ```c++
  boolean ret = kc868.writeSwitch(2,1);  //Turn ON relay2
  Serial.printf("open relay 2 ");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");
  ```

- **boolean writeSwitchAll(uint32_t value)**

  - Turn ON/OFF any 32 relays by one command. `uint32_t` of the parameter value, a total of 32 bits. The highest bit is the state of relay32, and the lowest bit is the state of relay1. If the value is 0x80000000, it means to Turn ON relay32, if the value of value is 0x00000080, it means to turn ON relay8. You just replace any bit with 1 or 0 for turn ON, turn OFF relay.
  - The return value is of type `boolean`, `true` means the operation succeeded, `false` means the operation failed.
  - Code example:
  
  ```c++
  uint32_t v = 0x00000001;
  boolean ret = kc868.writeSwitchAll( v );  //Turn ON relay1
  Serial.printf("open relay 1 ");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");
  ```

- **boolean writeSwitchAllON()**

  - Turn ON all 32 channel relays
  - The return value is of type `boolean`, `true` means the operation succeeded, `false` means the operation failed.
  - Code example:
  
  ```c++
  boolean ret = kc868.writeSwitchAllON();  //Turn ON all relays
  Serial.printf("open all relay");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");
  ```

- **boolean writeSwitchAllOFF()**

  - Turn OFF all 32 channel relays
  - The return value is of type `boolean`, `true` means the operation succeeded, `false` means the operation failed.
  - Code example:
  
  ```c++
  boolean ret = kc868.writeSwitchAllOFF();  //Turn OFF all relays
  Serial.printf("close all relay");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");
  ```

- **boolean readSensor(int idx)**

  - Read the state of the input sensor with the specified channel number. The range of the parameter 'idx' is 1-6, corresponding to 6 digital input ports.
  - The return value is of type `boolean`, `true` is triggered, `flase` is not triggered.
  - Code example:

  ```c++
  boolean ret = kc868.readSensor(1);  // read digital input1
  Serial.printf("io1 intput is");
  if(ret)
    Serial.println("tirgged!");
  else
    Serial.println("not tirgged.");
  ```

- **uint8_t readSensorAll()**

  - Read all digital input ports status
  - The return value is of type `uint8_t`, a total of 8 bits, take the lower 6 bits, from right to left, the 6th bit is the digital input6, and the lowest bit of the state is the state of the digital input1.
  - Code example:


  ```c++
  uint8_t value = kc868.readSensorAll();  //read 6 channel digital input ports
  Serial.printf("The intput state is %02x.\n",value);
  ```

- **void setSwitchChangeHook(hook func)**

  - Set the callback function when the relay state changed. The name of the callback function is defined as follows:

    `void SwitchUpdate(uint8_t idx, uint8_t state)`

   function has two parameters, the range of 'idx' is 1-32, corresponding to the channel number of the relay that has changed, the value of state is 0 or 1, 0 means to OFF, and 1 means to ON.
  - Code example:


  ```c++
  void SwitchUpdate(uint8_t idx, uint8_t state)
  {
    Serial.printf("relay %d is set to %d.\n",idx,state);
  }

  kc868.setSwitchChangeHook(SwitchUpdate);
  ```

- **void setSensorChangeHook(hook func)**

  - Set the callback function when the digital input changed. The name of the callback function is defined as follows:

    `void SwitchUpdate(uint8_t idx, uint8_t state)`

   function has two parameters, the range of 'idx' is 1-6, corresponding to the channel number of the digital input that has changed, the value of state is 0 or 1, 0 means triggered, and 1 means not triggered.
  - Code example:

  ```c++
  void SensorUpdate(uint8_t idx, uint8_t state)
  {
    Serial.printf("sensor %d is set to %d.\n",idx,state);
  }

  kc868.setSwitchChangeHook(SensorUpdate);
  ```

- **void setReadMode(uint8_t mode)**

  - Set the mode of reading the state of the relay, the value of mode is 0 or 1, '0' is directly read, and every time a query command is sent, it is read in real time. '1' is the read cache, the system reads the status of all relays every 2s and saves them. The value in memory is returned directly when the relay is read.
  - Note that `kc8688.setReadMode(0);` can be omitted without writing. Because default is mode=0
  - When using `kc868.setReadMode(1);`, calling `readSwitch()` will still return the value in memory. No need to call `readSwitchCache()`.
  - Code example:



  ```c++
  kc868.begin(115200);
  kc868.setReadMode(1);
  ```
  - To use the default mode, just use the following code：

  ```c++
  kc868.begin(115200);
  ```

