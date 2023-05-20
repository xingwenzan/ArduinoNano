//
// Created by 86159 on 2023-05-17.
//
#include "../lib/Ucglib/src/Ucglib.h"

#define TFT_CLK 13   // 26�� SCL (SPIʱ��)
#define TFT_MISO 12   // 25�� MISO (SPI����ӳ�)
#define TFT_MOSI 11   // 27�� MOSI (SPI��������)
#define TFT_CS   10   // 19�� CS (SPIƬѡ)
#define TFT_RST  9   // 23�� RESET (��λ)
#define TFT_DC   8   // 20�� RS (�Ĵ���ѡ��)

// For ILI9341 display
Ucglib_ILI9341_18x240x320_HWSPI ucg( TFT_DC, TFT_CS, TFT_RST);

void setup(void)
{
    delay(1000);
    ucg.begin(UCG_FONT_MODE_TRANSPARENT);
//    ucg.begin(UCG_FONT_MODE_SOLID);
    ucg.clearScreen();
}

void loop(void)
{
    //ucg.setRotate90();
    ucg.setFont(ucg_font_ncenR12_tr);
    ucg.setColor(255, 255, 255);
    //ucg.setColor(0, 255, 0);
    ucg.setColor(1, 255, 0,0);

    ucg.setPrintPos(50,25);
    ucg.print("Hello World!");


    delay(500);
}