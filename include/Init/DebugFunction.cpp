//
// Created by 86159 on 2023-05-13.
//
#include "../../lib/TM1638/src/TM1638.h"
#include "../../lib/U8g2_Arduino/src/U8g2lib.h"

//choose digital pins compatibles with your board
#define STB 7 // Strobe digital pin
#define CLK 6 // clock digital pin
#define DIO 5 // data digital pin

// 初始化 TM1638
TM1638 tm(CLK, DIO, STB);
// 初始化 0.96OLED
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

// 共阴 0-F
int gsmg_code[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
                     0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

void setup() {
    // 初始化 TM1638
    Serial.begin(115200);
    tm.reset();
    tm.test();
    // 初始化 0.96OLED
    u8g2.begin();
}

void loop() {
    uint8_t buttons = tm.getButtons();
    tm.writeLeds(buttons);
    for (int i = 0; i < 8; ++i) {
        if ((buttons >> i) & 1) {
            tm.displayDig(7-i, gsmg_code[i]);
        } else{
            tm.displayDig(7-i, 0x00);
        }
    }

    u8g2.clearBuffer();					// clear the internal memory 清除内部内存
    u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font 选择合适的字体
    u8g2.drawStr(0,10,"Hello World!");	// write something to the internal memory 将字符串"Hello World!"写入内部内存
    u8g2.setCursor(0,20);
    u8g2.print(buttons);
    u8g2.sendBuffer();					// transfer internal memory to the display 将内部内存的内容传输到显示屏上
}