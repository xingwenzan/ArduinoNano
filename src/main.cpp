#include <Arduino.h>
int i = HIGH;

void setup(void) {
    pinMode(13,OUTPUT);
    Serial.begin(9600);
}

void loop(void) {
    i=!i;
    digitalWrite(13,i);
    Serial.println(i);
    delay(100);
}

