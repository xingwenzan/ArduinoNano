//
// Created by 86159 on 2023-05-17.
//
#include "../lib/Ucglib/src/Ucglib.h"

#define TFT_CLK 13   // 26脚 SCL (SPI时钟)
#define TFT_MISO 12   // 25脚 MISO (SPI主入从出)
#define TFT_MOSI 11   // 27脚 MOSI (SPI主出从入)
#define TFT_CS   10   // 19脚 CS (SPI片选)
#define TFT_RST  9   // 23脚 RESET (复位)
#define TFT_DC   8   // 20脚 RS (寄存器选择)

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