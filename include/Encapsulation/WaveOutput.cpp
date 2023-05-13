//
// Created by 86159 on 2023-04-23.
//
#include <Arduino.h>
String op;
uint8_t m = 0;
bool judge= true;
void setup() {
// write your initialization code here
    Serial.begin(9600);
    Serial.println("请输入数据: ");
}

void loop() {
// write your code here
    if (Serial.available()){
        op = Serial.readString();
        Serial.println(op);
    }

    if (op.compareTo("W0\r")==0){
        if (judge){
            Serial.println(HIGH*255);
        } else{
            Serial.println(LOW);
        }
        m++;
        if (m==0){
            judge = !judge;
        }
    } else if (op.compareTo("W1\r")==0){
        uint8_t w = sin(m*PI/100)*125+125;
        m = (m+1)%200;
        Serial.println(w);
    } else if (op.compareTo("W2\r")==0){
        if (m==255){
            judge = false;
        } else if (m==0){
            judge = true;
        }
        Serial.println(m);
        if (judge){
            m++;
        } else{
            m--;
        }
    }
}