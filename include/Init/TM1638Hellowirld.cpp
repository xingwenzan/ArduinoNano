//
// Created by 86159 on 2023-05-13.
//
#include "../../lib/TM1638/src/TM1638.h"

//choose digital pins compatibles with your board
#define STB 7 // Strobe digital pin
#define CLK 6 // clock digital pin
#define DIO 5 // data digital pin

TM1638 tm(CLK,DIO,STB);

void setup() {
    Serial.begin(115200);
    tm.reset();
    tm.test();
}

void loop() {
    const uint8_t text[]={0x7c,0x1c,0x78,0x78,0x5c,0x54};
    for (uint8_t i=0;i<sizeof(text);i++)
        tm.displayDig(7-i, text[i]);

    uint8_t buttons = tm.getButtons();
    tm.writeLeds(buttons);

}