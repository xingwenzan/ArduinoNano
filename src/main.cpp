#include <Arduino.h>
int i = HIGH;

void setup(void) {
    pinMode(13,OUTPUT);
    Serial.begin(9600);
}

void loop(void) {
    double v;
    for (int j = 0; j < 1000; ++j) {
        v = 10*sin(j);
        analogWrite(13,v);
    }
}

