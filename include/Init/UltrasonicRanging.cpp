//
// Created by 86159 on 2023-05-23.
// https://blog.csdn.net/dg940350951/article/details/92803621
//

#include "Arduino.h"
#include "../lib/TimerOne/TimerOne.h"

// 定义引脚
const int trigPin = 2;
const int echoPin = 3;

// 定义超声波测距所需的变量
long duration;
int distance;

// 超声波测距
void measureDistance() {
    // 发送超声波信号
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // 接收回声并计算距离
    duration = pulseIn(echoPin, HIGH);
    distance = duration / 29.1 / 2;

    // 在串口监视器上显示结果
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}


void setup() {
    // 初始化串口通信
    Serial.begin(9600);

    // 设置引脚模式
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // 设置定时器中断时间间隔为2秒（即每隔2秒触发一次测量）
    Timer1.initialize(2000000); // 2秒对应的微秒数
    Timer1.attachInterrupt(measureDistance); // 绑定定时器中断处理函数
}

void loop() {
    // 其他任务可以放在这里，不会被定时器中断打断
}
