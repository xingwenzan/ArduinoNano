# ArduinoNano

为了电赛而学习的 ArduinoNano 板子开发

> 注意：项目运行前请复制粘贴到 main.cpp 中使用

## 目录及简介

- [主函数](src/main.cpp) &#x2002; 运行上传的是该文件，代码运行都烤到这里再运行
- [库文件](lib) &#x2002; 代码里可能用的但 `pio` 没有直接提供的库
  - [arduinoFFT](lib/arduinoFFT) &#x2002; `arduino` 官方的 `FFT` 函数，辅助实现频谱图
  - [Keypad](lib/Keypad) &#x2002; 控制 4*4 矩形键盘
  - [LiquidCrystal_I2C](lib/LiquidCrystal_I2C) &#x2002; 控制 1602LED 显示屏
  - [TM1638](lib/TM1638) &#x2002; 控制 TM1638(带独立按键、2*4 数码管、LED 灯)
  - [U8g2_Arduino](lib/U8g2_Arduino) &#x2002; 控制 0.96 OLED 显示屏
  - [TimerOne](lib/TimerOne) &#x2002; 定时器中断使用
- [项目代码](include) &#x2002; 一个文件代表一个任务的代码
  - [初始化类及基础功能类程序](include/Init)
    - [开关灯](include/Init/OpenAndCloseLED.cpp)
    - [LED 闪烁](include/Init/LedBlink.cpp)
    - [时钟模拟显示](include/Init/ClockDisplay.cpp)
    - [初始化 LED 屏幕及键盘](include/Init/InitLcdAndKeypad.cpp)
    - [初始化 TM1638](include/Init/TM1638Hellowirld.cpp)
    - [初始化 OLED](include/Init/U8g2Helloworld.cpp)
    - [程序监视显示 - Debug 辅助](include/Init/DebugFunction.cpp)
    - [定时器中断 - 原始版](include/Init/TimerInterruptOriginal.cpp)
    - [定时器中断 - 调用库版](include/Init/TimerInterruptLib.cpp)
    - [定时器控制超声波测距](include/Init/UltrasonicRanging.cpp)
    - [步进机](include/Init/StepperInit.cpp)
  - [一定功能封装程序](include/Encapsulation)
    - [方波输出](include/Encapsulation/SawtoothWave.cpp)
    - [串口波形输出](include/Encapsulation/WaveOutput.cpp)
    - [串口波形输出 - 含频谱图](include/Encapsulation/WaveformAndSpectrum.cpp)
    - [自定义波形串口输出](include/Encapsulation/CustomWaveInitial.cpp)
    - [新自定义波形串口输出 - 函数调整](include/Encapsulation/NewCustom.cpp)
    - [新控制自定义波形串口输出 - 键盘屏幕控制](include/Encapsulation/NewCtrlCustom.cpp)
  - [集成度较高型程序](include/Integrations)
    - [集成目录 - 时钟、闹钟、调时、串口波形发生、模拟秤](include/Integrations/ClockWaveAndScale.cpp)
    - [数字显示](include/Integrations/DigitalDisplay.cpp)
  - [通信类程序](include/Communication) &#x2002; 与 MSP432P401R 通信运行的程序，程序另一半见 MSP432P401R 项目同名程序
    - [LED 闪烁及串口通信](include/Communication/LedBlinkAndSerial.cpp)
  - [未调节成功程序](include/Tmp)