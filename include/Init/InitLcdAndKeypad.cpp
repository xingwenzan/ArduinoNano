//
// Created by 86159 on 2023-05-05.
//
#include "../../lib/Keypad/src/Keypad.h"
#include "../../lib/LiquidCrystal_I2C/LiquidCrystal_I2C.h"

// 设置 1602 液晶显示屏的 I2C 地址，宽度和高度
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4; // 4行
const byte COLS = 4; // 4列

// 定义键盘上的按键字符
char keys[ROWS][COLS] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
};

// 将行引脚连接到 Arduino Nano 的数字引脚
byte rowPins[ROWS] = {2, 3, 4, 5};

// 将列引脚连接到 Arduino Nano 的数字引脚
byte colPins[COLS] = {6, 7, 8, 9};

// 初始化键盘
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
    // 初始化串行通信
    Serial.begin(9600);

    // 初始化 LCD
    lcd.init();
    lcd.backlight();   // 背景光开
    lcd.setCursor(0, 0);   // 设置输出搁哪开始显示
    lcd.print("Key Pressed:");
}

void loop() {
    // 读取按键输入
    char key = keypad.getKey();

    if (key) {
        Serial.println(key);

        // 在 LCD 上显示按键字符
        lcd.setCursor(13, 1);
        lcd.print(key);
    }
}
