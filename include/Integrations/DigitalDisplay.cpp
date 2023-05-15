//
// Created by 86159 on 2023-05-15.
// https://blog.csdn.net/craftsman1970/article/details/106147208
//
#include "Arduino.h"

/*
 * 模块作用
 * DIO：数据输入/输出引脚，用于与单片机进行数据通信。
 * CLK：时钟引脚，用于控制数据传输的时序。
 * STB：片选引脚，用于选择 TM1638LED 模块，使其进入工作状态。
 *
 * 代码作用
 * 首先定义了 strobe、clk 和 dio 三个引脚的编号，用于控制 TM1638 模块的工作。
 * 然后定义了 writeByte 函数，用于向 TM1638 模块写入一个字节的数据。
 * 接着定义了 sendCommand 函数，用于向 TM1638 模块发送指令。
 * 定义了 reset 函数，用于初始化 TM1638 模块。
 * 定义了 readByte 函数，用于从 TM1638 模块读取一个字节的数据。
 * 定义了 readButtons 函数，用于读取 TM1638 模块上的按钮状态。
 * 定义了 setLed 函数，用于控制 TM1638 模块上指定位置的 LED 灯亮灭。
 * 定义了 showNumber 函数，用于在 TM1638 模块上显示一个数字。
 * 定义了 buttonLoop 函数，用于循环检测按钮状态，并更新 TM1638 模块上的 LED 灯状态。
 * 在 setup 函数中，初始化了 TM1638 模块。
 * 在 loop 函数中，循环显示数字和检测按钮状态。
 */

const int strobe = 7;
const int clk = 6;
const int dio = 5;

/*
 * 段选
 *
 * 解析
 * 首先设置 dio 引脚为输出模式，以便向 TM1638 模块发送数据。
 * 然后使用 for 循环遍历 8 次，每次传输一个字节的一个 bit。
 * 在每次传输 bit 的时候，先将 clk 引脚置为低电平，以便在下一步传输 bit 之前进行同步。
 * 然后判断当前 bit 的值，如果为 1，则将 dio 引脚置为高电平；否则将其置为低电平。
 * 将 value 右移一位，以便处理下一个 bit。
 * 将 clk 引脚置为高电平，以便传输下一个 bit。
 * 循环结束后，再将 clk 引脚置为低电平，以便进行下一轮传输。
 *
 * 原因
 * 这么实现的原因是因为 TM1638 模块使用的是串行通信协议，需要按照一定的时序和格式向其发送数据
 * 具体来说，每次向 TM1638 模块发送一个字节的数据时，需要先将 clk 引脚置为低电平，以便在下一步传输 bit 之前进行同步
 * 然后根据当前 bit 的值，设置 dio 引脚的电平；接着将 value 右移一位，以便处理下一个 bit；最后将 clk 引脚置为高电平，以便传输下一个 bit
 * 这样，就能够按照正确的格式向 TM1638 模块发送数据，并实现与其通信
 */
void writeByte(int value) {
    unsigned char i;
    pinMode(dio, OUTPUT);
    for (i = 0; i < 8; i++) {
        digitalWrite(clk, LOW);
        if (value & 0X01) {
            digitalWrite(dio, HIGH);
        } else {
            digitalWrite(dio, LOW);
        }
        value >>= 1;
        digitalWrite(clk, HIGH);
    }
    digitalWrite(clk, LOW);
}

/*
 * 首先将 strobe 引脚设为低电平，
 * 然后调用 writeByte 函数向 TM1638 模块写入指令，
 * 最后将 strobe 引脚设为高电平。
 */
void sendCommand(uint8_t value) {
    digitalWrite(strobe, LOW);
    writeByte(value);
    digitalWrite(strobe, HIGH);
}

/*
 * 初始化 TM1638 模块
 * 具体实现过程是：
 * 首先向 TM1638 模块发送指令 0x8b，用于设置灯亮度；
 * 然后发送指令 0x40，用于设置自动增量模式；
 * 接着将 strobe 引脚设为低电平，向 TM1638 模块写入 0xc0，用于设置起始地址为 0；
 * 然后循环 16 次，向 TM1638 模块写入 0x00，用于清空显示；
 * 最后将 strobe 引脚设为高电平。
 */
void reset() {
    sendCommand(0x8b); //set light,0x88-0x8f
    sendCommand(0x40); // set auto increment mode
    digitalWrite(strobe, LOW);
    writeByte(0xc0);   // set starting address to 0
    for (uint8_t i = 0; i < 16; i++) {
        writeByte(0x00);
    }
    digitalWrite(strobe, HIGH);
}

/*
 * 这个函数用于从 TM1638 模块读取一个字节的数据。
 * 具体实现过程是：
 * 首先将 dio 引脚设为输入模式，
 * 然后循环 8 次，每次将 value 右移一位，
 * 循环：
 * 然后将 clk 引脚设为低电平，延时 1 毫秒（如果 CPU 非常快，可能需要适当增加延时），
 * 然后判断 dio 引脚的状态，如果为高电平，则将 value 的最高位设为 1，否则设为 0，
 * 最后将 clk 引脚设为高电平。循环结束
 * 循环 8 次后，将 clk 引脚设为低电平，然后将 dio 引脚设为输出模式，并返回读取到的数据。
 */
int readByte(void) {
    unsigned char i;
    int value = 0;;
    pinMode(dio, INPUT);//set input mode.
    for (i = 0; i < 8; i++) {
        value >>= 1;
        digitalWrite(clk, LOW);
        delay(1);                           //if CUP is veryfast.
        if (digitalRead(dio) == HIGH)
            value |= 0x80;
        digitalWrite(clk, HIGH);
    }
    digitalWrite(clk, LOW);
    pinMode(dio, OUTPUT);//set output mode.
    return value;
}

/*
 * 用于读取 TM1638 模块上的按钮状态。
 * 具体实现过程是：
 * 首先将 strobe 引脚设为低电平，向 TM1638 模块发送读取按钮状态的指令 0x42；
 * 然后将 dio 引脚设为输入模式，循环 4 次，每次调用 readByte 函数读取一个字节的数据，并将其左移 i*2 位后与 buttons 做或运算，
 * 最后将 dio 引脚设为输出模式，并将 strobe 引脚设为高电平，返回读取到的按钮状态。
 */
int readButtons(void) {
    int buttons = 0;
    digitalWrite(strobe, LOW);
    pinMode(dio, OUTPUT);
    shiftOut(dio, clk, LSBFIRST, 0x42);
    pinMode(dio, INPUT);
    for (uint8_t i = 0; i < 4; i++) {
        int v = readByte() << i;
        buttons |= v;
    }
    pinMode(dio, OUTPUT);
    digitalWrite(strobe, HIGH);
    return buttons;
}

/*
 * 用于控制 TM1638 模块上指定位置的 LED 灯亮灭。
 * 具体实现过程是：
 * 首先将 strobe 引脚设为低电平，向 TM1638 模块发送设置 LED 灯状态的指令 0x44；
 * 然后将 strobe 引脚设为高电平，再将 strobe 引脚设为低电平，向 TM1638 模块写入 LED 灯的位置，
 * 最后向 TM1638 模块写入 LED 灯的状态，再将 strobe 引脚设为高电平。
 */
void setLed(int value, int position) {
    pinMode(dio, OUTPUT);
    sendCommand(0x44);
    digitalWrite(strobe, LOW);
    writeByte(0xC1 + (position << 1));
    writeByte(value);
    digitalWrite(strobe, HIGH);
}

/*
 * 用于在 TM1638 模块上显示一个数字。
 * 具体实现过程是：
 * 首先将数字拆分为每一位，存储在 digits 数组中；然后循环每一位，调用 setLed 函数设置对应位置的 LED 灯状态。
 */
void showNumber(int number) {
    const int digital[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    int pos = 7;
    while (pos >= 0) {
        digitalWrite(strobe, LOW);
        writeByte(0xc0 + pos * 2);
        if (number > 0) {
            int dig = number % 10;
            writeByte(digital[dig]);
            number /= 10;
        } else {
            writeByte(0);
        }
        digitalWrite(strobe, HIGH);
        pos--;
    }
}

/*
 * 这个函数用于循环检测按钮状态，并更新 TM1638 模块上的 LED 灯状态。
 * 具体实现过程是：
 * 首先调用 readButtons 函数读取按钮状态，然后循环 8 次，根据按钮状态设置对应位置的 LED 灯状态。
 */
void buttonLoop() {
    int buttons = readButtons();

    for (int position = 0; position < 8; position++) {
        int mask = 0x1 << position;
        setLed(buttons & mask ? 1 : 0, position);
    }
}

/*
 * 将 strobe、clk、dio 引脚设为输出模式，
 * 然后调用 reset 函数初始化 TM1638 模块的显示功能。
 */
void setup() {
    pinMode(strobe, OUTPUT);
    pinMode(clk, OUTPUT);
    pinMode(dio, OUTPUT);
    reset();
}

void loop() {
    for (int i = 0; i < 10000000; i++) {
        showNumber(i);
        buttonLoop();
    }
}