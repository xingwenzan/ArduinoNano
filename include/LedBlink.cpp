//
// Created by 86159 on 2023-04-22.
//
#include <Arduino.h>
void setup() {
// write your initialization code here
    pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
// write your code here
    digitalWrite(LED_BUILTIN,HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
}