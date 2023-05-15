//
// Created by 86159 on 2023-05-15.
//
#include "Arduino.h"

const int strobe = 7;
const int clk = 6;
const int dio = 5;
void writeByte(int value)     //write a byte.
{
    unsigned char i;
    pinMode(dio,OUTPUT);
    for(i=0;i<8;i++)
    {
        digitalWrite(clk,LOW);
        if(value&0X01){
            digitalWrite(dio,HIGH);
        }
        else{
            digitalWrite(dio,LOW);
        }
        value>>=1;
        digitalWrite(clk,HIGH);
    }
    digitalWrite(clk,LOW);
}
void sendCommand(uint8_t value)
{
    digitalWrite(strobe, LOW);
    writeByte(value);
    digitalWrite(strobe, HIGH);
}
void reset()
{
    sendCommand(0x8b); //set light,0x88-0x8f
    sendCommand(0x40); // set auto increment mode
    digitalWrite(strobe, LOW);
    writeByte(0xc0);   // set starting address to 0
    for(uint8_t i = 0; i < 16; i++)
    {
        writeByte(0x00);
    }
    digitalWrite(strobe, HIGH);
}
int readByte(void)          //read a byte.
{
    unsigned char i;
    int value=0;;
    pinMode(dio,INPUT);//set input mode.
    for(i=0;i<8;i++)
    {
        value>>=1;
        digitalWrite(clk,LOW);
        delay(1);                           //if CUP is veryfast.
        if(digitalRead(dio)==HIGH)
            value|=0x80;
        digitalWrite(clk,HIGH);
    }
    digitalWrite(clk,LOW);
    pinMode(dio,OUTPUT);//set output mode.
    return value;
}
int readButtons(void)
{
    int buttons = 0;
    digitalWrite(strobe, LOW);
    pinMode(dio, OUTPUT);
    shiftOut(dio, clk, LSBFIRST, 0x42);
    pinMode(dio, INPUT);
    for (uint8_t i = 0; i < 4; i++)
    {
        int v = readByte() << i;
        buttons |= v;
    }
    pinMode(dio, OUTPUT);
    digitalWrite(strobe, HIGH);
    return buttons;
}
void setLed(int value, int position)
{
    pinMode(dio, OUTPUT);
    sendCommand(0x44);
    digitalWrite(strobe, LOW);
    writeByte(0xC1 + (position << 1));
    writeByte(value);
    digitalWrite(strobe, HIGH);
}
void showNumber(int number)
{
    const int digital[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    int pos = 7;
    while(pos >=0){
        digitalWrite(strobe, LOW);
        writeByte(0xc0 + pos * 2);
        if(number > 0){
            int dig = number % 10;
            writeByte(digital[dig]);
            number /= 10;
        }
        else{
            writeByte(0);
        }
        digitalWrite(strobe, HIGH);
        pos--;
    }
}
void buttonLoop()
{
    int buttons = readButtons();

    for(int position = 0; position < 8; position++)
    {
        int mask = 0x1 << position;
        setLed(buttons & mask ? 1 : 0, position);
    }
}
void setup()
{
    pinMode(strobe, OUTPUT);
    pinMode(clk, OUTPUT);
    pinMode(dio, OUTPUT);
    reset();
}
void loop()
{
    for(int i = 0;i < 10000000; i++)
    {
        showNumber(i);
        buttonLoop();
    }
}