//
// Created by 86159 on 2023-05-08.
//
#include "Arduino.h"
#include "../../lib/LiquidCrystal_I2C/LiquidCrystal_I2C.h"

// 设置 1602 液晶显示屏的 I2C 地址，宽度和高度
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;

void display(){
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;
        seconds++;

        if (seconds == 60) {
            seconds = 0;
            minutes++;

            if (minutes == 60) {
                minutes = 0;
                hours++;

                if (hours == 24) {
                    hours = 0;
                }
            }
        }

        lcd.clear(); // 清屏
        lcd.setCursor(0, 0); // 设置光标位置
        if (hours < 10) lcd.print("0");
        lcd.print(hours);
        lcd.print(":");
        if (minutes < 10) lcd.print("0");
        lcd.print(minutes);
        lcd.print(":");
        if (seconds < 10) lcd.print("0");
        lcd.print(seconds);
    }
}

void setup() {
    // 初始化串行通信
    Serial.begin(9600);
    // 初始化 LCD
    lcd.init();
    lcd.backlight();
    // 在 LCD 上显示按键字符
    lcd.setCursor(0, 0);
}

void loop() {
    display();
}