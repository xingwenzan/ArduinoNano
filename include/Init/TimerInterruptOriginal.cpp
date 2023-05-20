//
// Created by 86159 on 2023-05-20.
// https://blog.csdn.net/TarshinHUANG/article/details/99580738
// https://blog.csdn.net/linZinan_/article/details/127832771
//
#include "Arduino.h"

volatile byte ledState = LOW;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    // 初始化 Timer1
    noInterrupts();           // 关闭所有中断，防止在设置定时器时被打断。
    // 初始化定时器1的控制寄存器和计数寄存器
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1A = 15624;            // 设置定时器的比较匹配值。当TCNT1（定时器的计数值）达到这个值时，定时器会产生一个中断。
    TCCR1B |= (1 << WGM12);   // CTC 模式。在这种模式下，当计数值（TCNT1）等于比较匹配值（OCR1A）时，计数器会自动清零。
    TCCR1B |= (1 << CS10) | (1 << CS12);  // 1024 预分频器。定时器的时钟频率是Arduino的时钟频率（16MHz）除以1024
    TIMSK1 |= (1 << OCIE1A);  // 使能定时器比较中断。当计数值（TCNT1）等于比较匹配值（OCR1A）时，这个中断会被触发。
    interrupts();             // 使能中断
}

ISR(TIMER1_COMPA_vect)      // 定时器中断服务程序
{
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
}

void loop() {
    // 主程序，中断服务程序会自动执行
}
