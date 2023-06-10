#include <Arduino.h>

void setup(void) {
    pinMode(A3,INPUT);
    Serial.begin(9600);
}

void loop(void) {
    int tmp = analogRead(A3);
    Serial.println(tmp);
    delay(1000);
}

