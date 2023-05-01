//
// Created by 86159 on 2023-04-23.
//
#include <Arduino.h>
uint8_t m;
void setup() {
// write your initialization code here
    Serial.begin(9600);
}

void loop() {
// write your code here
    Serial.println(m++);
    delay(1);
}