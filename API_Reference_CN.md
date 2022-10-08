# kc868-arduino-library  API Reference

[English](https://github.com/hzkincony/kc868-arduino-library/blob/main/API_Reference_EN.md)|[中文](https://github.com/hzkincony/kc868-arduino-library/blob/main/API_Reference_CN.md)

这个库是为ESP32系列编写的，暂时没有适配其他arduino单片机型号。使用这个库，只需要包含 `KC868.h` 这个文件，代码如下：

```C++
#include "KC868.h"
```

---

## 函数列表

- **KC868(HardwareSerial \*hs)**

  - KC868在使用之前需要创建一个全局对象才可以使用。创建完这个对象后，才可以使用下面的方法去实现功能。你可以使用`KC868()`来创建这个对象.代码如下：
  
  ```c++
  KC868 kc868(&Serial2);
  ```

  - 你需要传入一个串口对象的指针,在ESP32上，暂时只支持硬件串口.

- **boolean begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms, uint8_t rxfifo_full_thrhd)**

  - begin函数的作用是打开串口。他的参数和官方串口打开的参数一模一样。第一个波特率是必须传入的参数。其他的都可以省略。你只需要简单的使用下面的代码就可以完成初始化。
  
  ```c++
  kc868.begin(115200);
  ```

  - 如果你想使用其他IO口作为串口，可以使用下面的代码：

  ```c++
  kc868.begin(115200,SERIAL_8N1,22,23);
  ```

  - 其中22是接收引脚，23是发送引脚。

- **boolean close()**

  - 关闭KC868对应的串口。
  
- **void poll()**

  - 接收KC868的上报的数据并且定时下发查询报文，此函数必须被放置在`loop()`函数中。
  
- **boolean readSwitch(int idx)**

  - 读取指定序号的继电器状态，参数范围1-32,对应32路继电器。
  - 返回值是`boolean`类型，`true`是继电器打开状态，`flase`是关闭状态。
  - 代码示例：
  
  ```c++
  int ret = kc868.readSwitch(1);
  Serial.printf("The value of relay 1 is %d .\n",ret);
  ```

- **boolean readSwitchCache(int idx)**
  
  - 读取指定序号的继电器状态，参数idx的范围是1-32,对应32路继电器。
  - 返回值是`boolean`类型，`true`是继电器打开状态，`flase`是关闭状态.
  - 与`readSwitch`不同的是，这个直接返回内存中保存的继电器的值，而非实时读取。
  
- **uint32_t readSwitchAll()**

  - 读取32路继电器状态。
  - 返回值是`uint32_t`类型，一共32bit,最高位为继电器32的状态，最低位为继电器1的状态。如：返回值是0x00000001,则代表第1路继电器打开状态，返回值是0x00000003,代表第1号和第2号继电器都是打开状态。
  - 代码示例：
  
  ```c++
  uint32_t ret = kc868.readSwitchAll();
  Serial.printf("The value of all relay is %d .\n",ret);
  ```

- **boolean writeSwitch(int idx, int value)**

  - 设置指定序号的继电器状态。参数idx的范围是1-32，对应32路继电器；参数value的值为0和1,0是关闭，1是打开
  - 返回值是`boolean`类型，`true`表示操作成功，`false`表示操作失败。
  - 代码示例：
  
  ```c++
  boolean ret = kc868.writeSwitch(2,1);//打开第2路继电器
  Serial.printf("open relay 2 ");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");
  ```

- **boolean writeSwitchAll(uint32_t value)**

  - 设置所有32路继电器状态。参数value的`uint32_t`，共32bit位。最高位为继电器32的状态，最低位为继电器1的状态。如 value的值为0x80000000，表示打开第32路继电器，如value的值为0x00000080,表示打开第8路继电器。
  - 返回值是`boolean`类型，`true`表示操作成功，`false`表示操作失败。
  - 代码示例：
  
  ```c++
  uint32_t v = 0x00000001;
  boolean ret = kc868.writeSwitchAll( v );//打开第1路继电器
  Serial.printf("open relay 1 ");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");
  ```

- **boolean writeSwitchAllON()**

  - 同时打开32路继电器。
  - 返回值是`boolean`类型，`true`表示操作成功，`false`表示操作失败。
  - 代码示例：
  
  ```c++
  boolean ret = kc868.writeSwitchAllON();//打开所有继电器
  Serial.printf("open all relay");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");
  ```

- **boolean writeSwitchAllOFF()**

  - 同时关闭32路继电器。
  - 返回值是`boolean`类型，`true`表示操作成功，`false`表示操作失败。
  - 代码示例：
  
  ```c++
  boolean ret = kc868.writeSwitchAllOFF();//关闭所有继电器
  Serial.printf("close all relay");
  if(ret)
    Serial.println("success!");
  else
    Serial.println("fail.");
  ```

- **boolean readSensor(int idx)**

  - 读取指定序号的输入传感器状态，参数idx的范围是1-6,对应6路输入开关量。
  - 返回值是`boolean`类型，`true`是触发，`flase`是未触发.
  - 这里的返回值不是真实的电路电平，实际电路中，触发的电平的是电平。
  - 代码示例：

  ```c++
  boolean ret = kc868.readSensor(1);//读取开关量1的状态
  Serial.printf("io1 intput is");
  if(ret)
    Serial.println("tirgged!");
  else
    Serial.println("not tirgged.");
  ```

- **uint8_t readSensorAll()**

  - 读取所有输入传感器状态
  - 返回值是`uint8_t`类型，一共8bit，取低6位，从右往左，第6位为输入传感器6的状态，最低位为输入传感器1的状态。
  - 代码示例：

  ```c++
  uint8_t value = kc868.readSensorAll();//读取所有开关量的状态
  Serial.printf("The intput state is %02x.\n",value);
  ```

- **void setSwitchChangeHook(hook func)**

  - 设置继电器发生改变的回调函数，回调函数的名称定义如下：

    `void SwitchUpdate(uint8_t idx, uint8_t state)`

    函数有两个参数，idx的范围是1-32，对应发生改变的继电器的序号，state的值为0或1，0对应关闭，1对应打开。
  - 代码示例：

  ```c++
  void SwitchUpdate(uint8_t idx, uint8_t state)
  {
    Serial.printf("relay %d is set to %d.\n",idx,state);
  }

  kc868.setSwitchChangeHook(SwitchUpdate);
  ```

- **void setSensorChangeHook(hook func)**

  - 设置输入传感器发生改变的回调函数，回调函数的名称定义如下：

    `void SwitchUpdate(uint8_t idx, uint8_t state)`

    函数有两个参数，idx的范围是1-6，对应发生改变的输入传感器的序号，state的值为0或1，0对应未触发，1对应触发。
  - 代码示例：

  ```c++
  void SensorUpdate(uint8_t idx, uint8_t state)
  {
    Serial.printf("sensor %d is set to %d.\n",idx,state);
  }

  kc868.setSwitchChangeHook(SensorUpdate);
  ```

- **void setReadMode(uint8_t mode)**

  - 设置读取继电器输入状态的模式，mode的值为0或者1，0是直接读取，每次发送查询命令时，实时读取。1是读取缓存，系统每2s读取一次所有继电器的状态，并保存。读取继电器时直接返回内存中的值。
  - 注意 `kc8688.setReadMode(0);`可以省略不写。
  - 当使用 `kc868.setReadMode(1);`时，调用`readSwitch()`,仍然会返回内存中的值。无需调用`readSwitchCache()`。
  - 代码示例：

  ```c++
  kc868.begin(115200);
  kc868.setReadMode(1);
  ```
  - 使用默认模式则只需要使用下面的代码：

  ```c++
  kc868.begin(115200);
  ```

