//
// Created by 86159 on 2023-04-23.
//
#include <Arduino.h>
String op;
void setup() {
// write your initialization code here
    Serial.begin(9600);
    Serial.println("请输入数据: ");
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);
}

void loop() {
// write your code here
    if (Serial.available()){
        op = Serial.readString();
        Serial.println(op);
        if (op.compareTo("ON\r")==0){
            digitalWrite(LED_BUILTIN,HIGH);
            Serial.println("ON");
        }
        if (op.compareTo("OFF\r")==0){
            digitalWrite(LED_BUILTIN,LOW);
            Serial.println("OFF");
        }
    }
}