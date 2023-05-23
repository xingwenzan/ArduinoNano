//
// Created by 86159 on 2023-05-17.
//
#include "../lib/Ucglib/src/Ucglib.h"

#define TFT_CLK 13   // 26脚 SCL (SPI时钟)
#define TFT_SDA 11   // 28脚 SDA   （屏SPI模式下数据输入线）
#define TFT_CS   10   // 19脚 CS (SPI片选)
//#define TFT_RST  9   // 23脚 RESET (复位) *
//#define TFT_DC   8   // 20脚 RS (寄存器选择)

// For ILI9341 display
Ucglib_ILI9341_18x240x320_SWSPI ucg(TFT_CLK, TFT_SDA, TFT_CS);

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
    ucg.setColor(0, 0, 0);
    //ucg.setColor(0, 255, 0);
//    ucg.setColor(1, 255, 0,0);

    ucg.setPrintPos(50,50);
    ucg.print("Hello World!");

    delay(500);
}