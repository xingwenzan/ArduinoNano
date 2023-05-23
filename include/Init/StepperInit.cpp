//
// Created by 86159 on 2023-05-23.
// http://www.taichi-maker.com/homepage/reference-index/motor-reference-index/28byj-48-stepper-motor-intro/
// http://www.taichi-maker.com/homepage/reference-index/arduino-library-index/stepper-library/
// 注：Nano板的数字引脚无法提供足够的电流和功率来驱动步进电机
//

#include "Arduino.h"
#include "../lib/Stepper/src/Stepper.h"

// 定义步进电机对象
const int stepsPerRevolution = 2048;  // 28BYJ-48步进电机的步数
Stepper motor(stepsPerRevolution, 8, 10, 9, 11);  // 根据连接的引脚定义Stepper对象

void setup() {
    // 设置步进电机的速度
    motor.setSpeed(5);  // 设置转速为5 RPM
}

void loop() {
    // 顺时针旋转一个完整的圈
    motor.step(stepsPerRevolution);
    delay(1000);

    // 逆时针旋转半个圈
    motor.step(-stepsPerRevolution/2);
    delay(1000);
}
