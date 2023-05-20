//
// Created by 86159 on 2023-05-17.
//
#include "../lib/Ucglib/src/Ucglib.h"

// Please modify the pin numbers according to your wiring
#define SCK 13   // MOSI 27 SCLK 29 SCL26
#define SDA 11   // SDA 28
#define CS 10   // CS 19
#define DC 9   // RS 20
#define RESET 8   // RESET 23

Ucglib_ILI9341_18x240x320_HWSPI ucg(DC, CS, RESET);

void setup(void)
{
    delay(1000);
    ucg.begin(UCG_FONT_MODE_TRANSPARENT);
//    ucg.begin(UCG_FONT_MODE_SOLID);
    ucg.clearScreen();
    ucg.setFont(ucg_font_ncenB14_tr);

}

void loop(void)
{
    ucg.setColor(255, 255, 255); // White color
    ucg.setPrintPos(0, 20);
    ucg.print("Hello, world!");
    delay(2000);
    ucg.clearScreen();
}